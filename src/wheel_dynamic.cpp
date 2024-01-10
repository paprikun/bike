/*
 * wheel_dynamic.cpp
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */


#include "wheel_dynamic.h"
#include "keyboard.h"
wheel_dynamic wheel_dyn_1;
wheel_dynamic wheel_dyn_2;
wheel_dynamic rider_center;

wheel_dynamic rider_shoulders;



wheel_dynamic::wheel_dynamic(){

}
wheel_dynamic::wheel_dynamic(circle wheel_in, bool bw, float w)
{
	wheel = wheel_in;
	rotation_sense = 1;
	step = {0,0};
	step_contact = {0,0};
	pedal_rotation = {0,0};
	is_gravity = 0;
	angle = 0;
	back_wheel = bw;
	step_group_member = {0,0};
	weight = w;
	friction = {0,0};
	brake_power = 0;
}

//вычисление вектора движения при полном слипании со стеной или с отскоком; добавление вектора торможения (по вектору реакции опоры)
void wheel_dynamic::vector_step_recalc(checkwall res_from_wall)
{


	//вычисление проекции вектора движения на повержность (вектор скольжения)
	float lenA_long;

	point D = res_from_wall.cross_point;
	vec vA = {step.dx, step.dy};//вектор движения колеса
	vec vB = D - wheel.center;
	float lenA = len(vA);
	float lenB = len(vB);
	float scalar = vA.dx * vB.dx + vA.dy * vB.dy;
	float cosA = scalar / (lenA * lenB);
	lenA_long = (lenB / cosA);


	float k;
	if (lenA_long != 0)
		k = lenA / lenA_long;
	else{
		k = 0;
	}

	step = (wheel.center - D) * k + vA;

	//вычисление торможения
	if (brake_power){
		friction = vB * k * brake_power;
		friction = rotate_matrix(friction, 90) * rotation_sense;


		if (len(step) - len(friction) > 0.01){


			step = step - friction;

		}
		if (len(step) < len(friction)){

			step  = step * 0;

		}
	}
	//рассчет отскока
	if (abs(k) > 0.01){
	//	step =  step - vB * k * 1.00; // отскок 5%
	}


}


checkwall wheel_dynamic::check_of_cross() //поиск точки касания M, определение факта касания, нахождение линии которой коснемся
{
	checkwall ret;
	float x3 = wheel.center.x;
	float y3 = wheel.center.y;

	int size_ar = get_whalls_count();



	for (int i=0; i < size_ar; i++)
	{

		float x1 = lines[i].a.x;
		float x2 = lines[i].b.x;
		float y1 = lines[i].a.y;
		float y2 = lines[i].b.y;

		//находим точку пересечения перпендикуляра и линии
		float k = ((x3-x1) * (x2-x1) + (y3-y1)*(y2-y1))/ (pow(x2-x1, 2) + pow(y2-y1, 2));
		float x4 = x1 + k * (x2-x1);
		float y4 = y1 + k * (y2-y1);
		ret.cross_point = {x4, y4};

		float min_len = min(len(lines[i].a, wheel.center), len(lines[i].b, wheel.center));
		if (min_len < wheel.r){ //точка попадает на край отрезка

			if (min(len(lines[i].a, wheel.center), len(lines[i].b, wheel.center)) == len(lines[i].a, wheel.center))
				ret.cross_point = lines[i].a;
			else
				ret.cross_point = lines[i].b;

			ret.result = 1;
			ret.line = lines[i];
			ret.i = i;
			return ret;
		}

		float low_x = min(x1, x2);
		float high_x = max(x1, x2);
		float low_y = min(y1 ,y2);
		float high_y = max(y1, y2);

		if (low_x <= x4 && x4 <= high_x && low_y <= y4 && y4 <= high_y){//точка лежит на отрезке
			float D = sqrt(pow(x4 - x3, 2) + (pow (y4 - y3, 2)));
			if (D <= wheel.r){
				ret.result = 1;
				ret.line = lines[i];
				ret.i = i;
				return ret;
			}
		}


	}
	ret.i = -1;
	ret.result = 0;
	return ret;
}

//void print(vec pr){
//	printf("%f %f \n", pr.dx, pr.dy);
//}


void wheel_dynamic::pedal_power_use()
{
	//отработка силы педалей
	if (PedalPower!=0 && back_wheel){

		float len_step = sqrt(pow(step.dx ,2) + pow(step.dy ,2));
		if (len_step != 0){
			pedal_rotation.dx = PedalPower * (step.dx / len_step);
			pedal_rotation.dy = PedalPower * (step.dy / len_step);

			if (pedal_rotation.dy / PedalPower == 1 && pedal_rotation.dx == 0){
				pedal_rotation.dx = PedalPower;
			}
		}
	}
}

void wheel_dynamic::move_wheel(){





#if defined(__linux) || defined(__linux__)
	step.dy -= GRAVITY_STEP;
#else
	step.dy -= GRAVITY_STEP;//увеличиваем значение вектора падения
#endif

	step.dx += step_group_member.dx;
	step.dy += step_group_member.dy;



	point temp = wheel.center;
	if (is_gravity == 0){
		//step_inertion
		wheel.center.x += step.dx;
		wheel.center.y += step.dy;
	}
	is_gravity = 0;
	int i = 0;
	std::vector <int> walls_checked = {};

	while(++i){
		checkwall ch = check_of_cross();
		if (ch.result != 0){

			pedal_rotation = step_contact = {0, 0};
			wheel.center = temp; // возврат; смещение центра выше (step_inertion) анулируются тоже
			//сохраняем номера проверенных стен
			if ( std::find(walls_checked.begin(), walls_checked.end(), ch.i) != walls_checked.end() ){
				is_gravity = 0;
				break;
			}
			else{
				walls_checked.push_back(ch.i);
			}

			vector_step_recalc(ch);//Рассчет нового вектора движения


			step_contact = step;
			//рассчет направления вращения (1 по часовой)
			vec rad; //вектор от центра до точки касания с поверхностью
			rad.dx = ch.cross_point.x - wheel.center.x;
			rad.dy = ch.cross_point.y - wheel.center.y;
			//определяем по векторному произведению векторов направление вращения
			rotation_sense = (step.dx * rad.dy - step.dy * rad.dx) > 0 ? -1 : 1;
			pedal_power_use();
			step.dx += pedal_rotation.dx * (rotation_sense);
			step.dy += pedal_rotation.dy * (rotation_sense);

			wheel.center.x += step.dx;
			wheel.center.y += step.dy;
			is_gravity = 1;

		}else{
			break;
		}
	}
	if (is_gravity == 0){



		pedal_rotation = {0, 0};
	}




}

void wheel_dynamic::draw_wheel()
		//circle wheel, float* angle_in, vec step, vec step_contact, short rotation_sense)
{
	//float angle = *angle_in;
	if (back_wheel)
	glColor3d(1,0,0);

	glBegin(GL_LINE_LOOP );
    	for (int i=0; i<wheel.count; i++)
		{
    		glVertex2f(wheel.center.x  + wheel.r * cos(2 * M_PI * i / wheel.count + angle),
    				wheel.center.y  + wheel.r * sin(2 * M_PI * i / wheel.count + angle));

		}
    glEnd();
    glColor3d(0,0,0);
    {
	//спица
		glBegin(GL_LINES);
			glVertex2f(wheel.center.x, wheel.center.y);
			glVertex2f(wheel.center.x  + wheel.r * cos(angle), wheel.center.y  + wheel.r * sin(angle));
		glEnd();
    }
    if (0){
	//вектор движения
		glBegin(GL_LINES);
			glVertex2f(wheel.center.x, wheel.center.y);
			float len = sqrt(pow(step.dx, 2) + pow(step.dy, 2));
			float koef = 5 * wheel.r / len;
			glVertex2f(wheel.center.x  + step.dx * koef, wheel.center.y  + step.dy * koef);
		glEnd();

    }
    {
		float len_step = sqrt(pow(step_contact.dx, 2) + pow(step_contact.dy, 2));



		//if (sys_time_first == 0)
		//sys_time_first=clock();

		angle -= rotation_sense * (len_step) / (wheel.r);
    }
}



void wheel_dynamic::block_wheel(){

	float brake_abs_koef = 100.0; // антипробукс коэффциент. Определяет момент, когда колеса передейдут в скольжение.
								 //Грубо говоря момент, при котором сила тормозов превышает силу сцепления покрышки с поверхностью.
	if (len(step) / len(friction) < brake_abs_koef && len(friction) < len(step))
		step_contact.dx = step_contact.dy = 0;

}
