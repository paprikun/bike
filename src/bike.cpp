//============================================================================
// Name        : bike.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <time.h>

#define Wi 200
#define Hi 200
time_t sys_time_first =  0;//=clock();
time_t sys_time_second = 0;//=clock();
short rotation_sense_init = 1; //изначальное напрвление движения
float PedalPower = 0.0;
short rotation_sense = 1;
struct point{
	float x;
	float y;
};
struct line_type{
	point a;
	point b;
};
struct circle{
	point center;
	float r;
	int count;
};

struct checkwall{
	int i;
	point M;
	line_type line;
	bool result;
};
circle wheel = {{30, (5) * 20 + 50 + 3.5}, 7, 20};

float xx = wheel.r / sqrt(3);
line_type lines[] = {
		{{90,90},{95, 95}},
		{{75,56},{95, 56}},//пол до стены




					{{95,95},{95, 5}},//стена
					{{25,50},{15, 55}},//малый скат
					{{25,50},{51, 50}},//нижний пол
					{{52,50},{75, 50}},//нижний пол
					{{52,50},{52, 54}},//нижний пол
					{{15,58},{0, 73}},//первый скат
					{{15,58},{0, 73}},


//					{{30 + xx, 59},{30 + 2 * xx, 59 + wheel.r}},
//					{{30 - xx, 59},{30 - 2 * xx, 59 + wheel.r}},
//					{{30 - xx, 59},{30 + xx, 59}},

		    	};

bool key_down_on = 0;
bool key_up_on = 0;

struct vec {
	float dx;
	float dy;
};
vec proj = {0, 0};//вектор движения
vec proj_contact = {0, 0};//копия вектора движения при контакте с поверхностью

float len(point a, point b){
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
float len(vec a){
	return sqrt(pow(a.dx, 2) + pow(a.dy, 2));
}
float min(float a, float b){
	return a < b ? a : b;
}
float max(float a, float b){
	return a > b ? a : b;
}


//вычисление вектора движения при полном слипании со стеной
vec vector_projection(checkwall res_from_wall, float dx, float dy){

	float lenA_long;
	vec vA = {dx, dy};//вектор движения колеса

	float lenA = len(vA);
	point D = res_from_wall.M;

	{//попадание между краями отрезка
		vec vB = {D.x - wheel.center.x, D.y - wheel.center.y};
		float lenB = len(vB);
		float scalar = vA.dx * vB.dx + vA.dy * vB.dy;
		float cosA = scalar / (lenA * lenB);
		lenA_long = (lenB / cosA);
	}

	float k;
	if (lenA_long != 0)
		k = lenA / lenA_long;
	else{
		k = 0;
	}
	vec proj;
	{
		proj.dx = k * wheel.center.x + vA.dx - k * D.x;
		proj.dy = k * wheel.center.y + vA.dy - k * D.y;
	}
	return proj;
}


checkwall check_of_cross() //поиск точки касания M, определение факта касания, нахождение линии которой коснемся
{
	checkwall ret;
	float x3 = wheel.center.x;
	float y3 = wheel.center.y;

	int size_ar = sizeof(lines)/sizeof(lines[0]);



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
		ret.M = {x4, y4};

		float min_len = min(len(lines[i].a, wheel.center), len(lines[i].b, wheel.center));
		if (min_len < wheel.r){ //точка попадает на край отрезка

			if (min(len(lines[i].a, wheel.center), len(lines[i].b, wheel.center)) == len(lines[i].a, wheel.center))
				ret.M = lines[i].a;
			else
				ret.M = lines[i].b;

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
void print(vec pr){
	printf("%f %f \n", pr.dx, pr.dy);
}
int it = 0;
#define TOUCHES 3
void move_wheel(){
	static vec pedal_rotation = {0, 0};
	static bool is_gravity = 0;

	proj.dy -= 0.0002; // гравитация
	{
		point temp = wheel.center;
		if (is_gravity == 0){
			wheel.center.x += proj.dx;
			wheel.center.y += proj.dy;
		}
    	//цикл состоит из TOUCHES числа проверок

		is_gravity = 0;
       	checkwall ch;
       	int i = 0;
       	//printf("\n");

    	while(++i){
    		ch = check_of_cross();
            if (ch.result != 0){

        		pedal_rotation = {0, 0};
            	wheel.center = temp; // возврат
            	proj_contact = {0, 0};
            	if (i == TOUCHES){
            		printf("stop %d\n", it++);
            		is_gravity = 0;
            		break;
            	}
    			proj = vector_projection(ch, proj.dx, proj.dy);//Рассчет нового вектора движения
    			//рассчет направления вращения (-1 по часовой)
    			vec rad; //вектор от центра до точки касания с поверхностью
    			rad.dx = ch.M.x - wheel.center.x;
    			rad.dy = ch.M.y - wheel.center.y;

    			//определяем по векторному произведению векторов направление вращения
				rotation_sense = (proj.dx * rad.dy - proj.dy * rad.dx) > 0 ? 1 : -1;

    		    //отработка силы педалей
    			if (PedalPower!=0){

    				float len_proj = sqrt(pow(proj.dx ,2) + pow(proj.dy ,2));
    				if (len_proj != 0){
    					float pPower = PedalPower;
						pedal_rotation.dx = pPower * (proj.dx / len_proj);
						pedal_rotation.dy = pPower * (proj.dy / len_proj);
    				}
    			}
    			proj_contact = proj;
        		float pPower =  PedalPower;
    			if ((pedal_rotation.dx == 0 && pedal_rotation.dy == 0) || (pow(pedal_rotation.dx, 2) + pow(pedal_rotation.dy, 2) == pow(pPower, 2))){
    				pedal_rotation.dx = pPower * rotation_sense_init;
    				pedal_rotation.dy = 0;
    			}
    			proj.dx += pedal_rotation.dx * (-rotation_sense);
    			proj.dy += pedal_rotation.dy * (-rotation_sense);

    			wheel.center.x += proj.dx;
    	    	wheel.center.y += proj.dy;
    	    	is_gravity = 1;

            }else{
            	break;
            }
    	}
    	if (is_gravity == 0){
    		pedal_rotation = {0, 0};
    	}
	}
}
int cadrs = 0;
void draw_wheel(void)
{
	static float anlge = 0;

    glBegin(GL_LINE_LOOP );
    	for (int i=0; i<wheel.count; i++)
		{
    		glVertex2f(wheel.center.x  + wheel.r * cos(2 * M_PI * i / wheel.count + anlge),
    				   wheel.center.y  + wheel.r * sin(2 * M_PI * i / wheel.count + anlge));

		}
    glEnd();
//спица
    glBegin(GL_LINES);
    	glVertex2f(wheel.center.x, wheel.center.y);
    	glVertex2f(wheel.center.x  + wheel.r * cos(anlge), wheel.center.y  + wheel.r * sin(anlge));
    glEnd();
//вектор движения
//    glBegin(GL_LINES);
//    	glVertex2f(wheel.center.x, wheel.center.y);
//    	float len = sqrt(pow(proj.dx, 2) + pow(proj.dy, 2));
//    	float koef = 5*wheel.r / len;
//    	glVertex2f(wheel.center.x  + proj.dx * koef, wheel.center.y  + proj.dy * koef);
//    glEnd();


    float len_proj = sqrt(pow(proj_contact.dx, 2) + pow(proj_contact.dy, 2));


    cadrs++;
    if (wheel.center.y <= 57.00000 && sys_time_second == 0)
    {
    	sys_time_second = clock();
    	printf("falled, %ld, %d\n", (sys_time_second - sys_time_first)/1000, cadrs);
    }
    if (sys_time_first == 0)
    sys_time_first=clock();

    anlge += rotation_sense * (len_proj) / (wheel.r);
}
void draw_whalls(void)
{
	int size_ar = sizeof(lines)/sizeof(lines[0]);
	for (int i=0; i < size_ar; i++){
    glBegin(GL_LINES);
    	glVertex2f(lines[i].a.x, lines[i].a.y);
    	glVertex2f(lines[i].b.x, lines[i].b.y);
    glEnd();
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
		draw_wheel();
		move_wheel();
		//инерция нажатия клавиш
		if (key_down_on == 1 && key_up_on == 0){
			PedalPower = -0.0005; //крутим педали в обратную сторону
		}
		if (key_down_on == 0 && key_up_on == 1){
			PedalPower = 0.0005;
		}
		//glFlush();
		glutSwapBuffers();
		last_sys_time = sys_time;
	}
}
void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int x, int y) {


	switch(key) {
		case GLUT_KEY_DOWN :
			key_down_on = 1;
			break;
		case GLUT_KEY_UP :
			key_up_on = 1;
			break;
	}
}
void processSpecialKeysup(int key, int x, int y) {

	switch(key) {

		case GLUT_KEY_DOWN :
			PedalPower = 0;
			key_down_on = 0;
			break;
		case GLUT_KEY_UP :
			PedalPower = 0;
			key_up_on = 0;
			break;
	}
}

int main(int argc, char** argv)
{


	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    //glutInitWindowPosition(3000, 1000);
#if defined(__linux) || defined(__linux__)
    glutInitWindowPosition(3300, 900);
#else
    glutInitWindowPosition(500, 300);
#endif
    glutCreateWindow("my bike");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
 //   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
    glOrtho(0.0, Wi, 0.0, Hi, -1.0, 1.0);
 //   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glutDisplayFunc(renderScene);
	//glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);


    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutSpecialUpFunc(processSpecialKeysup);
    glutMainLoop();
	return 0;
}
