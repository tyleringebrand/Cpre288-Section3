/*
 * ping.c
 *
 *  Created on: Feb 28, 2020
 *      Author: ddc
 */

#include <ping.h>

volatile unsigned long last_time = 0;//rising edge
volatile unsigned long current_time = 0;//falling edge
volatile int update_flag = 0;
/**
 * init ping sensor
 */
void ping_init (void) {
    //init gpio
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRGPIO_R & 0x2) == 0); //wait for portB to turn on
    GPIO_PORTB_DIR_R |= 0x8; //pb3 is output
    GPIO_PORTB_AFSEL_R &= ~0x8; //pb3 is not alt input
    GPIO_PORTB_PCTL_R &= ~0x7000; //pb3 is not a clk signal
    GPIO_PORTB_DEN_R |= 0x8; //enable digital on pb3
    SYSCTL_RCGCTIMER_R |= 0x8; //turn on system clk
    while ((SYSCTL_PRTIMER_R & 0x8) == 0)
        ; //wait until timer 3 turns on

    TIMER3_CTL_R &= ~0x100;//disable timer 3b
    TIMER3_CFG_R |= 0x4;//make all timers 16bit
    TIMER3_TBMR_R |=0x7;//enable capture mode and edge time mode
    TIMER3_CTL_R |= 0xC00;//configure event type for pos and neg timer captures
    TIMER3_TBILR_R = 0xFFFF;//gives us a 24bit timer
    TIMER3_TBPR_R = 0xFF;//gives us a 24bit timer
    TIMER3_TBMR_R &= ~0x10;//force count down
    TIMER3_ICR_R = 0x400;//clear capture mode event interupt
    TIMER3_IMR_R |= 0x400;//enables interupts from timerB capture mode event

    //INTERUPT HANDLER
    //NVIC setup: set priority of timer 3b interrupt to 1
      NVIC_PRI9_R = (NVIC_PRI9_R & 0xFFFFFF1F) | 0x20;

      //NVIC setup: enable interrupt for timer3b
      NVIC_EN1_R |= 0x10;

      //tell CPU to use ISR handler for UART1 (see interrupt.h file)
      //from system header file: #define INT_UART1 22
      IntRegister(INT_TIMER3B, Timer3b_Handler);
      TIMER3_CTL_R |= 0x100;//enable timer 3b
      //globally allow CPU to service interrupts (see interrupt.h file)
      IntMasterEnable();
}

/**
 * turns on the PING sensor
 */
void ping_trigger (void) {
    TIMER3_CTL_R &= ~0x100;//enable timer 3b
    GPIO_PORTB_DIR_R |= 0x8; //pb3 is output
    GPIO_PORTB_AFSEL_R &= ~0x8;//pb3 is not alt input

    //send low-high-low starting pulse

    GPIO_PORTB_DATA_R &= ~0x8;//turn pb3 on low
    timer_waitMicros(5);//wait 5 micro sec
    GPIO_PORTB_DATA_R |= 0x8;//turn pb3 on high
    timer_waitMicros(5);//wait 5 micro sec
    GPIO_PORTB_DATA_R &= ~0x8;//turn pb3 on low

    //reconfig pb3 for input
    GPIO_PORTB_AFSEL_R |= 0x8;//pb3 is an alt input
    GPIO_PORTB_PCTL_R |= 0x7000;//pb3 is a clk signal
    TIMER3_CTL_R |= 0x100;//enable timer 3b
}

float ping_getDistance (unsigned long raw, float* ms) {
    //calculate distance
    float temp = (float)raw;
    temp /= 2;
    temp /= 16000.0;//temp is now in ms
    *ms = temp;
    temp *= 34.3;//temp is now in centimeters
    return temp;
}
/**
 * handles the interrupts from timer 3b
 */
void Timer3b_Handler(void)
{
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        last_time = current_time;
        current_time = TIMER3_TBR_R;
        update_flag++;
        TIMER3_ICR_R |= 0x400;
}

float ping_getData() {
    unsigned long time_diff;
        float dist;
        float ms;
        int overflow = 0;
        while(1) {
            ping_trigger();
            timer_waitMillis(2);//take a measurement every 500 ms
            if(update_flag >= 2) {
                time_diff = last_time - current_time;
                dist = ping_getDistance(time_diff, &ms);
                timer_waitMillis(500);//take a measurement every 500 ms
                update_flag = 0;
                return dist;
            }

        }
}


//int main(void) {
//    timer_init(); // Must be called before lcd_init(), which uses timer functions
//    lcd_init();
//    ping_init();
//    unsigned long time_diff;
//    float dist;
//    float ms;
//    int overflow = 0;
//    while(1) {
//        ping_trigger();
//        timer_waitMillis(2);//take a measurement every 500 ms
//        if(update_flag >= 2) {
//            time_diff = last_time - current_time;
//            dist = ping_getDistance(time_diff, &ms);
//            lcd_printf("Clk cycles-%lu\ntime in ms-%.5f\ndist in cm-%.5f\noverflow-%d", time_diff, ms, dist, overflow);
//            timer_waitMillis(500);//take a measurement every 500 ms
//            update_flag = 0;
//        }
//
//    }
//}
