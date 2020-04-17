/*
 * adc.h
 *
 *  Created on: Mar 1, 2016
 *      Author: nbergman
 */

#ifndef ADC_H_
#define ADC_H_

//#define IS_STEPPER_BOARD
//#define IS_STEPPER_BOARD

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "uart.h"

void adc_lookup_table (oi_t *sensor_data, int lookup[]);
int adc_read_convert(int lt[], int data);
void adc_init (void);
int adc_read (void);


#endif /* ADC_H_ */
