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
    SYSCTL_RCGCGPIO_R |= 0x2;//turns on portB
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
    TIMER1_TBILR_R = 0xBB80;//gives us a 24bit timer
    TIMER1_TBPR_R = 0x00;//gives us a 24bit timer
    TIMER1_TBMATCHR_R = 0x5DC0; //should be set to 90 degrees
    //0x5DC0 is 24,000 in integer
    TIMER1_TBPMR_R |= 0x0000;//shouldnt be needed
    TIMER1_CTL_R |= 0x100;//enable timer1b


}
/**
 * uses start and match values to position the servo
 */
int servo_move(float degrees) {

   float timeMicros = 1000.0 + (degrees/180.0)*(1000.0);  //must be at least 1000, linearly proportional to percent of 180 degrees we want to move
   int clockCycles = (int)(16 * timeMicros); //16 million cycles per second, thats 16 cycles per microsecond, multiply microseconds by 16

   TIMER1_CTL_R &= 0xFFFFFEFF;//timer off so we can change things,  change bit 8 to 0

   TIMER1_TBMATCHR_R = clockCycles; //converts to hexadecimal probably?
   //TIMER1_TBPMR_R = 0x0000; // we will never need prescale since max width is 2 ms, which is 4 hexadecimal values

   TIMER1_CTL_R |= 0x00000100;//timer on again, change bit 8 to 1

   timer_waitMillis(150);//allows time for the servo to move

   return 1;
}
