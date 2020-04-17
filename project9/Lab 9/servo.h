/*
 * servo.h
 *
 *  Created on: Apr 2, 2020
 *      Author: deema
 */

#ifndef SERVO_H_
#define SERVO_H_


#include <stdio.h>
#include <stdint.h>
#include "timer.h"
#include "lcd.h"
#include "tm4c123gh6pm.h"

//prototypes
void servo_init(void);
int servo_move(float degrees);


#endif /* PING_H_ */

