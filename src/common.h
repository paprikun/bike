/*
 * common.h
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */

#ifndef COMMON_H_
#define COMMON_H_



#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>    // std::find

struct point{
	float x;
	float y;
};

struct circle{
	point center;
	float r;
	int count;
};


struct vec {
	float dx;
	float dy;
};
struct line_type{
	point a;
	point b;
};
#if defined(__linux) || defined(__linux__)
float KEY_STEP = 0.0005;
float GRAVITY_STEP = 0.0002;
#else
#define KEY_STEP 0.00002
#define GRAVITY_STEP  0.0008
#endif
vec operator + (const vec& v1, const vec& v2);
vec operator - (const vec& v1, const vec& v2);
vec operator - (const point& p1, const point& p2);
point operator + (const point& p1, const vec& p2);
vec operator * (const vec& v1, const float& del);
vec operator / (const vec& v1, const float& del);


vec delta_ret(point a_1, point a_center, float k);
float len(point a, point b);
float len(vec a);
float min(float a, float b);
float max(float a, float b);
float vec_cos_angle(vec v1, vec v2);
point rotate_matrix(point A3, point A2, float angle);
vec rotate_matrix(vec A, float angle);



void print_args(std::vector <float> args);
#endif /* COMMON_H_ */
