int servo_move(float degrees)
{

   float timeMicros = 1000.0 + (degrees/180.0)*1000.0;  //must be at least 1000, linearly proportional to percent of 180 degrees we want to move
   int clockCycles = (int)(16* timeMicros); //16 million cycles per second, thats 16 cycles per microsecond, multiply microseconds by 16

   TIMER1_CTL_R &= 0xFFFFFEFF;//timer off so we can change things,  change bit 8 to 0

   TIMER0_TBMATCHR_R = clockCycles; //converts to hexadecimal probably?
   TIMER0_TBPMR_R = 0x0000; // we will never need prescale since max width is 2 ms, which is 4 hexadecimal values

   TIMER1_CTL_R |= 0x00000100;//timer on again, change bit 8 to 1
}
