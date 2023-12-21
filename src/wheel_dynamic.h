/*
 * wheel_dynamic.h
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */

#ifndef WHEEL_DYNAMIC_H_
#define WHEEL_DYNAMIC_H_
#include "common.h"
#include "map.h"
struct checkwall{
	int i;
	point cross_point;
	line_type line;
	bool result;
};
class wheel_dynamic{

	public:
		wheel_dynamic();

		wheel_dynamic(circle wheel_in, bool bw, float w);

	private:

		short rotation_sense;
		vec step_contact;//копия вектора движения при контакте с поверхностью
		vec pedal_rotation;
		bool is_gravity;//факт касания с поверхностью
		float angle;//угол спицы

	public:
		circle wheel;
		vec friction; //вектор торможения
		vec step;//вектор движения
		vec step_group_member;//вектор связки

		float weight;
		bool back_wheel;//ведущее колесо, имеющее привод


		float brake_power;//усилие тормоза



		void block_wheel();
		void draw_wheel();
		void move_wheel();
		void pedal_power_use();
		checkwall check_of_cross();
		void vector_step_recalc(checkwall res_from_wall);

};




extern wheel_dynamic wheel_dyn_1;
extern wheel_dynamic wheel_dyn_2;
extern wheel_dynamic head_dyn;

#endif /* WHEEL_DYNAMIC_H_ */
