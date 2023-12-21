/*
 * edges.h
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */

#ifndef EDGES_H_
#define EDGES_H_

#include "wheel_dynamic.h"
#include "common.h"

#define EDGE_1_LEN_MIN 15.0
#define EDGE_1_LEN_MAX 33.0
#define EDGE_2_LEN_MIN 15.0
#define EDGE_2_LEN_MAX 33.0
#define EDGE_COUNT_STEP 200
#define EDGE_1_LEN_STEP ((EDGE_1_LEN_MAX - EDGE_1_LEN_MIN) / EDGE_COUNT_STEP)
#define EDGE_2_LEN_STEP ((EDGE_2_LEN_MAX - EDGE_2_LEN_MIN) / EDGE_COUNT_STEP)
#define EDGE_1_LEN_AVER ((EDGE_1_LEN_MAX - EDGE_1_LEN_MIN)/2 + EDGE_1_LEN_MIN)
#define EDGE_2_LEN_AVER ((EDGE_2_LEN_MAX - EDGE_2_LEN_MIN)/2 + EDGE_2_LEN_MIN)


#define EDGES_COUNT 3
struct edge{
	wheel_dynamic* A;
	wheel_dynamic* B;
	float length;

};
vec calc_step_of_one_member_in_pair(wheel_dynamic* wheel_d_1, wheel_dynamic* wheel_d_2, float init_len_1_center, float LENGTH);
void connect_to_group();
extern edge edges[EDGES_COUNT];//массив ребер
#endif /* EDGES_H_ */
