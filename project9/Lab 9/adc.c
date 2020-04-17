/*
 * adc.c
 *
 *  Created on: Feb 28, 2020
 *      Author: ddc
 */

#include <adc.h>

void adc_init (void) {
    //turn on portB
    SYSCTL_RCGCGPIO_R |= 0x2;
    while ((SYSCTL_PRGPIO_R & 0x2 ) == 0 ); //wait for portB to turn on
    GPIO_PORTB_DIR_R &= ~0x10; //pb4 is input
    GPIO_PORTB_AFSEL_R |= 0x10;//pb4 is alt input
    GPIO_PORTB_DEN_R &= ~0x10;//disable digital on pb4
    GPIO_PORTB_AMSEL_R |= 0x10;//makes pb4 analog

    //start adc initialization

    SYSCTL_RCGCADC_R |= 0x1;//turn on ADC0
    while ((SYSCTL_PRADC_R & 0x1 ) == 0 ); //wait for adc0 to turn on
    ADC0_ACTSS_R &= ~0x8; //disable ss3
    ADC0_EMUX_R &= ~0xF000;//ss3 is software trig
    ADC0_SSMUX3_R &= ~0x000F;//clear ss3
    ADC0_SSMUX3_R += 10; //set channel to ain10
    ADC0_SSCTL3_R = 0b0110;//
    ADC0_SAC_R |= 0x4;//hardware averaging
    ADC0_ACTSS_R |= 0x8;//turn on ss3
}


int adc_read (void) {
    unsigned long result;

    ADC0_PSSI_R |= 0x8; //turn on ss3
    while((ADC0_RIS_R & 0x8) == 0){};//wait for ss3 to turn on

    result = ADC0_SSFIFO3_R & 0xFFF; //grab data out of fifo

    ADC0_ISC_R = 0x8; // reset flags
    return result;
}

void adc_lookup_table(oi_t *sensor_data, int lookup[]) {

    move_back(sensor_data, 100);
    lookup[0] = adc_read();

//    int savg = 0;


    int i = 0;
    for(i = 0; i < 9; i++) {
        timer_waitMillis(1000);
        move_back(sensor_data, 50);
        timer_waitMillis(1000);


        lookup[i] = adc_read();
        //delete later
        char snum[10];
        sprintf(snum, "%d\t%d", i, lookup[i]);
        uart_sendStr(snum);

    }

    //ret = lookup;
}

//converts the quantization value into cm
int adc_read_convert(int lt[], int data) {
    //find what indexes the value is between
    if(data >= lt[0]){
        return 9;
    }

    int i = 1;
    for(i = 1; i < 9; i++){
        double slope;
        int dist;
        
        double b;

        if(data >= lt[i]){

            //slope = -((lt[i] - lt[i - 1]));
            // MIGHT BE RIGHTslope = (((double)lt[i - 1] + (double)data) / (double)lt[i - 1]) - 1;
           // slope = 5 * slope;
           // dist = slope + 10 + ((i-1) * 5);
           // dist = (slope*(i - 1)) * 5 +10;

        	slope = 5.0/((double)lt[i] - (double)lt[i-1]);// slope = rise/run
        	b = (i * 5) + 10 - (slope * lt[i]); //b = y - mx
        	dist = slope*data + b; // y = mx + b
            return dist;
        }


    }
    return 51;
}

//int main(void) {
//    timer_init(); // Must be called before lcd_init(), which uses timer functions
//    lcd_init();
//    adc_init();
//    uart_init();
//    oi_t *sensor_data = oi_alloc();// do this once at start of main
////    oi_init(sensor_data);//do this only once at start of main
//
//
//    int bacd[9] = {2650, 1855, 1460, 1170, 1020, 880, 775, 710, 645};//bacd must always be size 9
////    adc_lookup_table(sensor_data, bacd);
//
//    while(1){
//        int data = adc_read();
//        int dist = adc_read_convert(bacd, data);
//        lcd_printf("%d    %d", data, dist);
//
//        //send the data to putty as well
//        char snum[10];
//        sprintf(snum, "%d   ,   %d", data, dist);
//        uart_sendStr(snum);
//
//        timer_waitMillis(400);
//
//    }
//    oi_free(sensor_data);
//}
