

/**
 * main.c
 */
#include "servo.h"

int main(void)
{
    servo_init();
    servo_move(30);
    servo_move(150);
    servo_move(90);
}
