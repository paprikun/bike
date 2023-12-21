/*
 * common.cpp
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */

#include "common.h"







vec operator + (const vec& v1, const vec& v2)
{

	vec result;
	result.dx = v1.dx + v2.dx;
	result.dy = v1.dy + v2.dy;
	return result;

}
vec operator - (const vec& v1, const vec& v2)
{

	vec result;
	result.dx = v1.dx - v2.dx;
	result.dy = v1.dy - v2.dy;
	return result;

}
vec operator - (const point& p1, const point& p2)
{

	vec result;
	result.dx = p1.x - p2.x;
	result.dy = p1.y - p2.y;
	return result;

}
point operator + (const point& p1, const vec& p2)
{

	point result;
	result.x = p1.x + p2.dx;
	result.y = p1.y + p2.dy;
	return result;

}
vec operator * (const vec& v1, const float& del)
{

	vec result;
	result.dx = v1.dx * del;
	result.dy = v1.dy * del;
	return result;

}

vec operator / (const vec& v1, const float& del)
{

	vec result;
	if (del == 0)
		return {0,0};
	result.dx = v1.dx / del;
	result.dy = v1.dy / del;
	return result;

}

vec delta_ret(point a_1, point a_center, float k){

	return (a_1 - a_center) - (a_1 - a_center) * k;
}
float len(point a, point b){
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
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

float vec_cos_angle(vec v1, vec v2){

	float temp = v1.dx * v2.dx + v1.dy * v2.dy;
	float len_v1 = len(v1);
	float len_v2 = len(v2);
	float result = temp / (len_v1 * len_v2);
	return result;

}
vec rotate_matrix(vec A, float angle){

// M =  cos(A), -sin(A)
//	    sin(A), cos(A)
// P1 = M * P
// A = 90

	vec P = A;
	float rad_angle = angle / 180 * M_PI;
	vec P1 = {P.dx * cos(rad_angle) -  P.dy * sin(rad_angle),
			  P.dx * sin(rad_angle) +  P.dy * cos(rad_angle)};
	return P1;
}

point rotate_matrix(point A3, point A2, float angle){

// M =  cos(A), -sin(A)
//	    sin(A), cos(A)
// P1 = M * P
// A = 90

	float rad_angle = angle / 180 * M_PI;

	vec P = A2 - A3;
	vec P1 = {P.dx * cos(rad_angle) -  P.dy * sin(rad_angle),
			  P.dx * sin(rad_angle) +  P.dy * cos(rad_angle)};
	return A3 + P1;
}
void print_args(std::vector <float> args){

	for(float n : args)
		printf("%f  ", n);
	printf("\n");
}
