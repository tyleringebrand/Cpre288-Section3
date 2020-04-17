/*
 * movement.h
 *
 *  Created on: Jan 31, 2020
 *
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"

//prototypes
double move_forward(oi_t *sensor_data, double distance_mm);
double move_back(oi_t *sensor_data, double distance_mm);
double turnLeft(oi_t *sensor_data, double degrees);
double turnRight(oi_t *sensor_data, double degrees);

#endif /* MOVEMENT_H_ */
