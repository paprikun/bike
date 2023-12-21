/*
 * edges.cpp
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */
#include "edges.h"

edge edges[EDGES_COUNT];//массив ребер
vec calc_step_of_one_member_in_pair(wheel_dynamic* wheel_d_1, wheel_dynamic* wheel_d_2, float init_len_1_center, float LENGTH){

	vec step0 = {0,0};
	vec step1 = {0,0};
	//рассчет основного парного вектора, когда одно колесо мгновенно передает импульс другому.

	vec AB = wheel_d_2->wheel.center - wheel_d_1->wheel.center;
	{
		float cosAM1 = vec_cos_angle(wheel_d_1->step, AB);

		if (len(wheel_d_1->step) == 0)
			cosAM1 = 0;

		float lenAM1 = cosAM1 * len(wheel_d_1->step);
		float kAM1 = lenAM1 / len(AB);
		vec step_AM  = (wheel_d_2->wheel.center - wheel_d_1->wheel.center) * kAM1;
		vec BA = wheel_d_1->wheel.center - wheel_d_2->wheel.center;
		float cosBM2 = vec_cos_angle(wheel_d_2->step, BA);

		if (len(wheel_d_2->step) == 0)
				cosBM2 = 0;

		float lenBM2 = cosBM2 * len(wheel_d_2->step);
		float kBM2 = lenBM2 / len(BA);
		vec BM2 = (wheel_d_1->wheel.center - wheel_d_2->wheel.center) * kBM2;

		vec arithmetic_average = (step_AM + BM2) / 2;
		step0 = (arithmetic_average  - step_AM)  ;
		if (cosAM1 == 0)
			step0 = {0,0};

		step0 = step0 / wheel_d_1->weight;
	}
	if (round(len(AB) * 100) != round(LENGTH * 100)){
		//рассчет поправочного вектора, когда расстояние между колесами изменилось в меньшую или большую сторону

		//находим центр тяжести между точками
		point center_point;
		center_point.x = wheel_d_1->wheel.center.x + (wheel_d_2->wheel.center.x - wheel_d_1->wheel.center.x) * init_len_1_center;
		center_point.y = wheel_d_1->wheel.center.y + (wheel_d_2->wheel.center.y - wheel_d_1->wheel.center.y) * init_len_1_center;

		float len_1_2 = len(wheel_d_1->wheel.center , center_point);
		float k_1_2 = abs(len_1_2) / (init_len_1_center * LENGTH);
		step1 = (delta_ret(wheel_d_1->wheel.center, center_point, k_1_2));
		//printf("%f step1_added \n", len(AB) - LENGTH);

			if ((wheel_d_1 == &wheel_dyn_1 && wheel_d_2 == &wheel_dyn_2)||(wheel_d_1 == &wheel_dyn_2 && wheel_d_2 == &wheel_dyn_1))
				return (step0 / 2 + step1 / 1) / 1;//ошибочный коэффициент для рамы

//k1 = 2 k2 = 64 супер медлленный отскок
//k1 = 1 k2 = 64 медлленный отскок
//k1 = 1 k2 = 32 средений отскок
//k1 = 1 k2 = 16 быстрый отскок
//k1 = 0.5 k2 = 16 очень быстрый отскок
//медленный отскок - легко заземлять, тяжело вылетать с вылета
//медленный отскок - тяжело заземлять, легко вылетать с вылета


			float k1 = (2);
			float k2 = 64;


			//return (step0 / 64 + step1 / 32) / 1;
			return (step0 * k1 + step1 / k2) ;//ошибочный коэффициент для райдер - байк


	}
	return (step0) ;//ошибочный коэффициент
}


void connect_to_group(){

//	edges[i++] = {&wheel_dyn_1, &head_dyn, 0};
//	edges[i++] = {&wheel_dyn_2, &head_dyn, 0};


	for (int i=0; i<EDGES_COUNT; i++){
		(edges[i].A)->step_group_member = {0,0};
		(edges[i].B)->step_group_member = {0,0};
	}

	for (int i=0; i<EDGES_COUNT; i++){


		float part_weight_A = 1 - (edges[i].A)->weight / ((edges[i].A)->weight + (edges[i].B)->weight);
		float part_weight_B = 1 - (edges[i].B)->weight / ((edges[i].A)->weight + (edges[i].B)->weight);

		(edges[i].A)->step_group_member = (edges[i].A)->step_group_member + calc_step_of_one_member_in_pair(edges[i].A, edges[i].B,   part_weight_A, edges[i].length);
		(edges[i].B)->step_group_member = (edges[i].B)->step_group_member + calc_step_of_one_member_in_pair(edges[i].B, edges[i].A,   part_weight_B, edges[i].length);
	}

}
