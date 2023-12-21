/*
 * keyboard.h
 *
 *  Created on: 20 дек. 2023 г.
 *      Author: user
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#include "common.h"
#include "wheel_dynamic.h"
#include "edges.h"





void processNormalKeys(unsigned char key, int x, int y);
void processNormalKeysup(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void processSpecialKeysup(int key, int x, int y);

void keyboard_inertion();
extern float PedalPower;
extern int key_f5_on;
void keyboard_init();
#endif /* KEYBOARD_H_ */
