

/**
 * main.c
 */
#include "object_detection.h"

int main(void)
{
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    ping_init();
    servo_init();
    adc_init();
    uart_init();
    oi_t *sensor_data = oi_alloc();// do this once at start of main
    oi_init(sensor_data);//do this only once at start of main

    int ltadc[9] = {2650, 1855, 1460, 1170, 1020, 880, 775, 710, 645};//look up table for adc to properly get distance must be changed for new cyBot
    int d;
    servo_move(0);//makes sure the servo is in the correct position before starting
    timer_waitMillis(50);//give the servo a little extra time to move since it could be moving from 180

	int IR[181];
	float ping[181];
	int sensorAgreement[181];
	float ErrorTolerance = .1;  //percent
	int degreesPerMeasurement = 2; // minimum 1

    char msg[40];
    sprintf(msg, "Degrees\tIR Distance (cm)\tSonar Distance (cm)");
    uart_sendStr(msg);//displays msg onto putty
    for( d = 0; d <= 180; d += degreesPerMeasurement) {//takes measurements from 0degrees to 180
		int index = d / degreesPerMeasurement;

		int adcData = adc_read();//IR sensor reads data
        int adcDist = adc_read_convert(ltadc, adcData);//raw data is converted into centimeters
        float pingData = ping_getData();//gets the distance calculated from the ping sensor

        sprintf(msg, "%d, %d, %f", d, adcDist, pingData);
        uart_sendStr(msg);


		IR[index] = adcDist;
		ping[index] = pingData;
		if((IR[index] >= ping[index] * (1.0- ErrorTolerance)) && (IR[index] <= ping[index] * (1.0 + ErrorTolerance)))
			sensorAgreement[index] = 1; // within tolerance
		else
			sensorAgreement[index] = 0; //out of tolerance

        //compare data to notice object

        servo_move((float)d);//moves to d degrees
    }

    //move servo to point in the middle of object
	

	////find object based on true readings
	int index = 0;
	while (sensorAgreement[index] == 0)
		index++;
	//index now points to first occurance of a confirmed data point
	int startingIndex = index;
	while (sensorAgreement[index] == 1)
		index++;
	//index now points to first  occurence of an unconfirmed point after the object
	int endingIndex = index-1;
	//middle index of object, used for radius
	int middleIndex = ( endingIndex + startingIndex) / 2;

	//width in degrees equals degrees per scan * number of confirmed scans
	int widthDegrees = (endingIndex - startingIndex)*degreesPerMeasurement;
	//True width result = (2Pi * distanceAway) * (arcWidth/360)
	double differenceCM = (2 * 3.14159 * IR[middleIndex])*((double)widthDegrees / 360.0);



    oi_free(sensor_data);
}
