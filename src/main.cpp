//============================================================================
// Name        : bike.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "common.h"
#include "wheel_dynamic.h"
#include "edges.h"
#include "keyboard.h"
#include "bike.h"

#define Wi 600
#define Hi 300
time_t sys_time_first =  0;
time_t sys_time_second = 0;





void graph_init(){

//нужна авто-проверка, если точек будет много!!
//можно убирать некоторые связи


	float wb = (float) WHEEL_BASE / 100 * 2;
	wheel_dyn_1 = wheel_dynamic({{30, 145 + 100 + WHLR + 0}, WHLR, 20}, 1, 1);
	wheel_dyn_2 = wheel_dynamic({{30 + wb, 145 + 100 + WHLR + 0}, WHLR, 20}, 0, 1);
	rider_center 	= wheel_dynamic({{45, 145 + 100 + WHLR + 17}, 4, 20}, 0, 13);



	edges[0] = {&wheel_dyn_1, &wheel_dyn_2, 0};
	edges[0].length = wb;

	edges[1] = {&wheel_dyn_1, &rider_center, 0};
	edges[1].length = EDGE_1_LEN_AVER;

	edges[2] = {&wheel_dyn_2, &rider_center, 0};
	edges[2].length = EDGE_2_LEN_AVER;




}


point wheel_dyn_1_center_telemetry;
time_t sys_time_telemetry;
bool telemetry_timer_started = 0;
int telemetry_timer = 0;


void telemetry(){

	if (telemetry_timer_started == 0){

		sys_time_telemetry=clock();
		telemetry_timer_started = 1;
		wheel_dyn_1_center_telemetry = wheel_dyn_1.wheel.center;

	}
	if (clock() - sys_time_telemetry >= 100){

		float wheel_dyn_1_center_len = 0.5 * len(wheel_dyn_1.wheel.center, wheel_dyn_1_center_telemetry);
		wheel_dyn_1_center_telemetry = wheel_dyn_1.wheel.center;
		telemetry_timer += 100;
		printf("%6.3f s   %6.3f m/s \n", (float)telemetry_timer / 1000 , wheel_dyn_1_center_len);//1м = 20
		sys_time_telemetry = clock();
	}

}
void renderScene(void)
{
	static time_t last_sys_time = 0;

	time_t sys_time=clock();
#if defined(__linux) || defined(__linux__)
	if (sys_time - last_sys_time >  1000)
#else
		if (sys_time - last_sys_time > 1)
#endif

	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glLoadIdentity();
		draw_whalls();


		connect_to_group();





		wheel_dyn_1.draw_wheel();
		wheel_dyn_2.draw_wheel();
		bike.draw_bike();

		rider_center.draw_wheel();


		wheel_dyn_1.move_wheel();
		wheel_dyn_2.move_wheel();
		rider_center.move_wheel();


		telemetry();



		keyboard_inertion();

		if (key_f5_on == 1){
			key_f5_on = 2;
		 	keyboard_init();
			graph_init();
		}

		//glFlush();
		glutSwapBuffers();
		last_sys_time = sys_time;
	}
}



int main(int argc, char** argv)
{


  	glutInit(&argc, argv);

 	keyboard_init();
	graph_init();

    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1000, 500);
    //glutInitWindowPosition(3000, 1000);
#if defined(__linux) || defined(__linux__)
    glutInitWindowPosition(3300, 900);
#else
    glutInitWindowPosition(0, 300);
#endif
    glutCreateWindow("my bike");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
 //   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
    glOrtho(0, Wi, 0.0, Hi, -1.0, 1.0);
 //   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glutDisplayFunc(renderScene);
	//glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);


    glutKeyboardFunc(processNormalKeys);
    glutKeyboardUpFunc(processNormalKeysup);

    glutSpecialFunc(processSpecialKeys);
    glutSpecialUpFunc(processSpecialKeysup);

    glutMainLoop();
	return 0;
}
