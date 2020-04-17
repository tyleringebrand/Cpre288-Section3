/*
 * Demetrius
 * Daniel Z
 *
 * movement.c
 *
 *  Created on: Jan 31, 2020
 *
 */

#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"


//prototypes
double move_forward(oi_t *sensor_data, double distance_mm);
double move_back(oi_t *sensor_data, double distance_mm);
double turnLeft(oi_t *sensor_data, double degrees);
double turnRight(oi_t *sensor_data, double degrees);


//void main() {
//    lcd_init();
//    timer_init();
//    oi_t *sensor_data = oi_alloc();// do this once at start of main
//    oi_init(sensor_data);//do this only once at start of main
//
//
//    move_forward(sensor_data, 2000);//moves forward 1m
//
//    oi_free(sensor_data);
//}


/**
 * The main function that moves both wheels forward at different speeds because of bad hardware, and continues to move
 * forward until the Roomba either hits the required distance or one of its bumpers hits something, after which it will move back 15cm,
 * turn a direction 90 degrees, move forward 25 cm, right its direction to be facing the one it was originally, and finally continue to travel forward the rest of the distance.
 */
double move_forward(oi_t *sensor_data, double distance_mm) {
    double sum = 0;
    oi_setWheels(300,280);//move at full speed
        while(sum < distance_mm) {

            oi_update(sensor_data);
            sum += sensor_data->distance;
            lcd_printf("%lf", sum);

            if(sensor_data->bumpRight){
               move_back(sensor_data, 150);
               turnLeft(sensor_data, 90);
               move_forward(sensor_data, 250);
               turnRight(sensor_data,90);
               move_forward(sensor_data, distance_mm - sum);
               return sum;
            } else if (sensor_data->bumpLeft) {
               move_back(sensor_data, 150);
               turnRight(sensor_data, 90);
               move_forward(sensor_data, 250);
               turnLeft(sensor_data,90);
               move_forward(sensor_data, distance_mm - sum);
               return sum;
            }
            oi_update(sensor_data);
        }

    oi_setWheels(0,0);//stops wheels

    return sum;
}

/**
 * This method is made for moving the Roomba backwards, while being able to be called in move_forward which is the main method
 */
double move_back(oi_t *sensor_data, double distance_mm) {
    double sum = 0;
    oi_setWheels(-25,-25);//move at full speed
        while(sum < distance_mm) {
            oi_update(sensor_data);
            sum += abs(sensor_data->distance);
            //lcd_printf("%lf", sum);
        }
    oi_setWheels(0,0);//stops wheels

    return sum;
}

/**
 * this method turns the Roomba right a certain amount of degrees, and calculates this angle with the angle value from the sensor_data
 * the angle value is adjusted for slight error when turning
 */
double turnRight(oi_t *sensor_data, double degrees) {
    double sumR = 0;
    degrees -= 3;
    degrees += abs(sensor_data->angle);
    oi_setWheels(-40,40);
        while(abs(sumR) < abs(degrees)) {
            oi_update(sensor_data);
            sumR += sensor_data->angle;
        }

    oi_setWheels(0,0);//stops wheels

    return sumR;
}


/**
 * this method turns the Roomba left a certain amount of degrees, and calculates this angle with the angle value from the sensor_data
 * the angle value is adjusted for slight error when turning
 */
double turnLeft(oi_t *sensor_data, double degrees) {
    double sumL = 0;
    degrees -= 3;//this corrects error in the turn
    degrees -= abs(sensor_data->angle);
    oi_setWheels(25,-25);
        while(abs(sumL) < abs(degrees)) {
            oi_update(sensor_data);
            sumL += sensor_data->angle;
        }

    oi_setWheels(0,0);//stops wheels

    return sumL;
}
