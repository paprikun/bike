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

//	float chainstay = frame_chainstay * k;
//	float stack = frame_stack * k;
//	float reach = frame_reach * k;
//	float headtube = frame_headtube * k;
//	float seattube = frame_seattube * k;
//	float HT_angle = frame_HT_angle;
//	float ST_angle = frame_ST_angle;
//	float bb_drop = frame_bb_drop * k;


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

    glColor3d(0,0,0);


}
