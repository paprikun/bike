/*
 * bike.cpp
 *
 *  Created on: 26 дек. 2023 г.
 *      Author: user
 */

#include "bike.h"

bike_dynamic bike;


float bike_dynamic::x_p(point pp) {
	return ((pp.x - p1.x) * cosa - (pp.y - p1.y) * sina + p1.x);
}
float bike_dynamic::y_p(point pp) {
	return ((pp.x - p1.x) * sina + (pp.y - p1.y) * cosa + p1.y);
}


bike_dynamic::bike_dynamic(){

	float k = 0.02;
	chainstay = frame_chainstay * k;
	stack = frame_stack * k;
	reach = frame_reach * k;
	headtube = frame_headtube * k;
	seattube = frame_seattube * k;
	HT_angle = frame_HT_angle;
	ST_angle = frame_ST_angle;
	bb_drop = frame_bb_drop * k;

	cosa = 1;
	sina = 0;

	star = {0, 0, system_star_rad * k, 10};
	stick_1 = system_stick_1 * k;
	stick_2 = system_stick_2 * k;
	hip  = rider_hip * k;
	knee  = rider_knee * k;

	torso  = rider_torso * k;
	hand  = rider_hand * k;


}
// Функция рассчета и прорисовки ломаной линии, состоящей из двух отрезов, расположенных под углом друг к другу таким образом, чтобы свободные концы отрезков были на расстоянии ab
// точки a и b - свободные концы отрезков
// точка с - общая
// type - расположение сгиба относительно отрезка ab
// type = -1 вершина 'c' лежит по часовой относительно вектора ab
// type = 1 вершина 'c' лежит против часовой относительно вектора ab

//Пример
// point a = {100, 100};
// point b = {100, 200};
// float len_ac = 77;
// float len_bc = 77;
// triangle(a, b, len_ac, len_bc, 1);

bool triangle(point a, point b, float len_ac, float len_bc, int type){

	float len_ab = len(a, b);

	//Обработка ошибки
	if (len_ab >= len_ac + len_bc)
	{
		return 0;
	}

	float cos_a = (pow(len_ac, 2) + pow(len_ab, 2) - pow(len_bc, 2)) / (2  * len_ac * len_ab);
	float sin_a = sqrt(1 - pow(cos_a, 2));

	vec ab = {b.x - a.x, b.y - a.y};
	vec ac_temp;
	ac_temp.dx = 		  cos_a * ab.dx - (type) * sin_a * ab.dy;
	ac_temp.dy = (type) * sin_a * ab.dx + cos_a * ab.dy;

	float len_ac_temp = len(ac_temp);
	float k = len_ac / len_ac_temp;
	vec ac;
	ac = ac_temp * k;
	point c = a + ac;

	glBegin(GL_LINES );
	glVertex2f(a.x, a.y);
	glVertex2f(c.x, c.y);
	glEnd();

	glBegin(GL_LINES );
	glVertex2f(c.x, c.y);
	glVertex2f(b.x, b.y);
    glEnd();
    return 1;

}


void bike_dynamic::draw_bike(){

	int inv = 1;
	point bw1 = wheel_dyn_1.wheel.center;
	point bw2 = wheel_dyn_2.wheel.center;

	if (wheel_dyn_2.back_wheel){
		bw1 = wheel_dyn_2.wheel.center;
		bw2 = wheel_dyn_1.wheel.center;
		inv = -1;
	}



	cosa = (bw2.x - bw1.x) / len(bw1, bw2);
	sina = (bw2.y - bw1.y) / len(bw1, bw2);


	p1 = {bw1.x, bw1.y};
	point p2 = {p1.x  + chainstay, p1.y - bb_drop * inv};
	point p3 = {(float)(p2.x - seattube * cos(ST_angle / 180 * M_PI)), (float)(p2.y + seattube * sin(ST_angle / 180 * M_PI) * inv)};
	point p4 = {p2.x + reach, p2.y + stack * inv};
	point p5 = {(float)(p4.x + headtube * cos(HT_angle / 180 * M_PI)), (float)(p4.y - headtube * sin(HT_angle / 180 * M_PI) * inv)};
	point p6 = {bw2.x, bw2.y};

	glBegin(GL_LINE_LOOP );
		glVertex2f(p1.x, p1.y);
		glVertex2f(x_p(p2), y_p(p2));
		glVertex2f(x_p(p5), y_p(p5));
		glVertex2f(x_p(p4), y_p(p4));
		glVertex2f(x_p(p3), y_p(p3));

    glEnd();

	glBegin(GL_LINES );
	glVertex2f(x_p(p2), y_p(p2));
		glVertex2f(x_p(p3), y_p(p3));
    glEnd();

	glBegin(GL_LINES );
		glVertex2f(x_p(p4), y_p(p4));
		glVertex2f(p6.x, p6.y);

    glEnd();



    //передняя звезда
    star.center = p2;
    float star_angle = 0;
	glBegin(GL_LINE_LOOP );
    	for (int i=0; i<star.count; i++)
		{
    		point temp = {(float)(star.center.x  + star.r * cos(2 * M_PI * i / star.count + star_angle)), (float)(star.center.y  + star.r * sin(2 * M_PI * i / star.count + star_angle))};
    		glVertex2f(x_p(temp), y_p(temp));

		}
    glEnd();

    //шатуны
    //{
	glBegin(GL_LINES);
		point temp1 = {star.center.x, star.center.y};
		point temp2 = {star.center.x  + stick_1 * cos(star_angle), star.center.y  + stick_1 * sin(star_angle)};
		glVertex2f(x_p(temp1), y_p(temp1));
		glVertex2f(x_p(temp2), y_p(temp2));

	glEnd();
    //}
    //{
    	glBegin(GL_LINES);
		point temp3 = {star.center.x, star.center.y};
		point temp4 = {star.center.x  - stick_1 * cos(star_angle), star.center.y  - stick_1 * sin(star_angle)};
		glVertex2f(x_p(temp3), y_p(temp3));
		glVertex2f(x_p(temp4), y_p(temp4));
	glEnd();
    //}
    //райдер
	//нужна функция, которая по координатам 2х точек и условию выпуклый или вогнутый, длинне 2х ребер, найдет координаты 3й точки треугольника. А так же возвращать ошибку если треугольник не существует.
    //ноги
   // {


//		triangle (rider_center.wheel.center, temp2, hip, knee, 1);
//		triangle (rider_center.wheel.center, temp4, hip, knee, 1);
//		triangle (rider_center.wheel.center, p4, torso, hand, 1);





    glColor3d(0,0,0);


}
