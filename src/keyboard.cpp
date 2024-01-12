/*
 * kyeboard.cpp
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */




#include "keyboard.h"
bool key_down_on = 0;
bool key_up_on = 0;
bool key_left_on = 0;
bool key_right_on = 0;
bool key_space_on = 0;
float PedalPower = 0.0;
int key_f5_on = 0;
void processNormalKeys(unsigned char key, int x, int y) {

	switch(key) {

	case 27:
		exit(0);
		break;

	case 32: //пробел

		if (key_space_on == 0){
			PedalPower = 0;




			key_space_on = 1;
		}
		break;
	}
}

void processNormalKeysup(unsigned char key, int x, int y) {
	switch(key) {
	case 32: //пробел
		key_space_on = 0;
		break;
	}
}
void processSpecialKeys(int key, int x, int y) {


	switch(key) {

		case GLUT_KEY_F5:
			if (key_f5_on == 0){
				key_f5_on = 1;


			}
			break;
		case GLUT_KEY_UP :
			key_up_on = 1;
			break;

		case GLUT_KEY_DOWN :
			key_down_on = 1;

			//держат горку около 40 градусов (при условии что тормозов 2).
			//Для 1го тормоза заднего нужен коэффициент 4. Тормозной путь зависит от силы, с которой колесо давит на поверхность и этого коэффициента.

			wheel_dyn_1.brake_power = 0.9;
			wheel_dyn_2.brake_power = 0.9;


			break;

		case GLUT_KEY_LEFT :



			key_left_on = 1;
			break;
		case GLUT_KEY_RIGHT :
			key_right_on = 1;
			break;


	}
}
void processSpecialKeysup(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_F5:
			key_f5_on = 0;
			break;

		case GLUT_KEY_UP :
			PedalPower = 0;
			key_up_on = 0;
			break;
		case GLUT_KEY_DOWN :
			PedalPower = 0;
			key_down_on = 0;

			wheel_dyn_1.brake_power = 0;
			wheel_dyn_2.brake_power = 0;


			break;



		case GLUT_KEY_LEFT :
			key_left_on = 0;
			break;
		case GLUT_KEY_RIGHT :
			key_right_on = 0;
			break;



	}
}

void keyboard_inertion(){

	//инерция нажатия клавиш
	if (key_down_on == 1 && key_up_on == 0){
		if (wheel_dyn_1.brake_power)
			wheel_dyn_1.block_wheel();
		if (wheel_dyn_2.brake_power)
			wheel_dyn_2.block_wheel();

	}
	if (key_down_on == 0 && key_up_on == 1){
		if (wheel_dyn_1.back_wheel){

			if (abs(PedalPower) < 1)
				PedalPower += KEY_STEP;

		}else{
			if (abs(PedalPower) < 1)
			PedalPower -= KEY_STEP;
		}
	}

	if(1)
	if (key_left_on == 0 && key_right_on == 0){



			if(edges[1].length < EDGE_1_LEN_AVER && edges[1].length + EDGE_1_LEN_STEP < EDGE_1_LEN_AVER){
				edges[1].length += EDGE_1_LEN_STEP;
				edges[2].length -= EDGE_2_LEN_STEP;
			}





			if(edges[1].length > EDGE_1_LEN_AVER && edges[1].length - EDGE_1_LEN_STEP > EDGE_1_LEN_AVER){
				edges[1].length -= EDGE_1_LEN_STEP;
				edges[2].length += EDGE_2_LEN_STEP;
			}




	}


	if (key_left_on == 1 && key_right_on == 0){



			if(edges[1].length > EDGE_1_LEN_MIN){
				edges[1].length -= EDGE_1_LEN_STEP;
				edges[2].length += EDGE_2_LEN_STEP;
			}


	}
	if (key_left_on == 0 && key_right_on == 1){



			if(edges[1].length < EDGE_1_LEN_MAX){
				edges[1].length += EDGE_1_LEN_STEP;
				edges[2].length -= EDGE_2_LEN_STEP;
			}


	}


}
void keyboard_init(){
	key_down_on = 0;
	key_up_on = 0;
	key_left_on = 0;
	key_right_on = 0;
	key_space_on = 0;
	PedalPower = 0.0;
	//key_f5_on = 0;
}
