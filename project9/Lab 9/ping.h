/*
 * ping.h
 *
 *  Created on: Mar 1, 2016
 *      Author: nbergman
 */

#ifndef PING_H_
#define PING_H_

//#define IS_STEPPER_BOARD
//#define IS_STEPPER_BOARD

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"


void ping_init (void); //initialize the timer
void ping_trigger (void); //activate the PING sensor
float ping_getDistance (unsigned long raw, float* ms); //calculate distance
void Timer3b_Handler(void);
float ping_getData();


#endif /* PING_H_ */
