/*
 * servo.c
 *
 *  Created on: Apr 2, 2020
 *      Authors: Demetrius Christou,
 */

#include <servo.h>

/**
 * initializes the servo for use with the PMW mode
 * the servo should start at 90degrees
 */
void servo_init(void) {

    //init gpio Port B
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRGPIO_R & 0x2) == 0); //wait for portB to turn on
    GPIO_PORTB_DEN_R |= 0x20; //enable digital on pb5
    GPIO_PORTB_DIR_R |= 0x20; //pb5 is output
    GPIO_PORTB_AFSEL_R |= 0x20;//set PB5 to alt function
    GPIO_PORTB_PCTL_R |= 0x700000;//PB5 function is Timer 1B PWM mode


    //init timer 1b
    SYSCTL_RCGCTIMER_R |= 0x2; //turn on timer 1
    while ((SYSCTL_PRTIMER_R & 0x2) == 0); //wait until timer 3 turns on
    TIMER1_CTL_R &= ~0x100;//disable timer1b
    TIMER1_CFG_R |= 0x4;//selects 16bit timer mode
    TIMER1_TBMR_R |= 0xA;//enables pwm mode and periodic timer mode
    TIMER1_TBMR_R &= ~0x4;//turns on edge-count mode
    TIMER1_TBILR_R = 0xFFFF;//gives us a 24bit timer
    TIMER1_TBPR_R = 0xFF;//gives us a 24bit timer
    //TIMER1_TBMATCHR ?= ?????;//should set the servo to 90
    //TIMER1_TBPMR ?= ?????;//should set the servo to 90
    TIMER1_CTL_R |= 0x100;//enable timer1b


}
/**
 * uses start and match values to position the servo
 */
int servo_move(float degrees) {
    return 0;
}
