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
#include "bike.h"

#define wb1  ((float) WHEEL_BASE / 100 * 2)

extern float EDGE_0_LEN_AVER_START;
extern float EDGE_1_LEN_AVER_START;
extern float EDGE_2_LEN_AVER_START;
extern float EDGE_3_LEN_AVER_START;
extern float EDGE_4_LEN_AVER_START;

#define EDGE_0_LEN_MIN (EDGE_1_LEN_AVER_START * 1)
#define EDGE_0_LEN_MAX (EDGE_1_LEN_AVER_START * 1)

#define EDGE_1_LEN_MIN (EDGE_1_LEN_AVER_START * 0.9)
#define EDGE_1_LEN_MAX (EDGE_1_LEN_AVER_START * 1.1)

#define EDGE_2_LEN_MIN (EDGE_2_LEN_AVER_START * 0.9)
#define EDGE_2_LEN_MAX (EDGE_2_LEN_AVER_START * 1.1)

#define EDGE_3_LEN_MIN (EDGE_3_LEN_AVER_START * 1)
#define EDGE_3_LEN_MAX (EDGE_3_LEN_AVER_START * 1)

#define EDGE_4_LEN_MIN (EDGE_4_LEN_AVER_START * 1)
#define EDGE_4_LEN_MAX (EDGE_4_LEN_AVER_START * 1)

#define EDGE_COUNT_STEP 500

#define EDGE_0_LEN_STEP ((EDGE_0_LEN_MAX - EDGE_0_LEN_MIN) / EDGE_COUNT_STEP)
#define EDGE_1_LEN_STEP ((EDGE_1_LEN_MAX - EDGE_1_LEN_MIN) / EDGE_COUNT_STEP)
#define EDGE_2_LEN_STEP ((EDGE_2_LEN_MAX - EDGE_2_LEN_MIN) / EDGE_COUNT_STEP)
#define EDGE_3_LEN_STEP ((EDGE_3_LEN_MAX - EDGE_3_LEN_MIN) / EDGE_COUNT_STEP)
#define EDGE_4_LEN_STEP ((EDGE_4_LEN_MAX - EDGE_4_LEN_MIN) / EDGE_COUNT_STEP)


#define EDGE_0_LEN_AVER ((EDGE_0_LEN_MAX - EDGE_0_LEN_MIN)/2 + EDGE_0_LEN_MIN)
#define EDGE_1_LEN_AVER ((EDGE_1_LEN_MAX - EDGE_1_LEN_MIN)/2 + EDGE_1_LEN_MIN)
#define EDGE_2_LEN_AVER ((EDGE_2_LEN_MAX - EDGE_2_LEN_MIN)/2 + EDGE_2_LEN_MIN)
#define EDGE_3_LEN_AVER ((EDGE_3_LEN_MAX - EDGE_3_LEN_MIN)/2 + EDGE_3_LEN_MIN)
#define EDGE_4_LEN_AVER ((EDGE_4_LEN_MAX - EDGE_4_LEN_MIN)/2 + EDGE_4_LEN_MIN)


#define EDGES_COUNT 5
struct edge{
	wheel_dynamic* A;
	wheel_dynamic* B;

	float length;

};
void edges_init();
vec calc_step_of_one_member_in_pair(wheel_dynamic* wheel_d_1, wheel_dynamic* wheel_d_2, float init_len_1_center, float LENGTH);
void connect_to_group();
extern edge edges[EDGES_COUNT];//массив ребер
#endif /* EDGES_H_ */
