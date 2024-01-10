/*
 * bike.h
 *
 *  Created on: 26 дек. 2023 г.
 *      Author: user
 */

#ifndef BIKE_H_
#define BIKE_H_
#include "common.h"
#include "wheel_dynamic.h"

#define WHEEL_BASE 1052
#define frame_chainstay  385
#define frame_stack  	 571
#define frame_reach 	 410
#define frame_headtube   80
#define frame_seattube   337
#define frame_HT_angle   69
#define frame_ST_angle   74
#define frame_bb_drop    25

#define system_stick_1	 175
#define system_stick_2	 system_stick_1
#define system_star_rad	 50

#define rider_hip 450
#define rider_knee 450

#define WHLR 6.77
class bike_dynamic{
public:
	bike_dynamic();
	void draw_bike();
protected:
	float x_p(point pp);
	float y_p(point pp);
	float cosa;
	float sina;
	point p1;

	float chainstay;
	float stack;
	float reach;
	float headtube;
	float seattube;
	float HT_angle;
	float ST_angle;
	float bb_drop;

	float stick_1;
	float stick_2;
	circle star;

	float hip;
	float knee;




};
extern bike_dynamic bike;
#endif /* BIKE_H_ */
