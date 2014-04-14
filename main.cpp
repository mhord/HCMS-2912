#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hcms/hcms.h"
#include "SPI/SPI.h"
#include "gpio/gpio.h"

#define RS_PIN 2
#define RESET_PIN 4
#define BLANK_PIN 3

int main()
{
	printf("Hello!");
	gpio RS(RS_PIN, OUTPUT);
	gpio RESET(RESET_PIN, OUTPUT);
	gpio BLANK(BLANK_PIN, OUTPUT);
	gpio csPin(10, OUTPUT);
	hcms display(24, &RESET, &RS, &BLANK, &csPin);
	display.initDisplay();
	display.clearDisplay();
	unsigned char hours = 0;
	unsigned char mins = 0;
	unsigned char secs = 0;
	int msecs = 0;
	char time[11];
	while(1)
	{
		sprintf(time, "%d:%.2d:%.2d.%.3d", hours, mins, secs, msecs++);
		BLANK.pinWrite(HIGH);
		for (unsigned int i=0;i<strlen(time); i++)
		{
			display.printChar(time[i]);
		}
		for (unsigned int i=0; i<24-strlen(time); i++)
		{
			display.printChar(' ');
		}	
		BLANK.pinWrite(LOW);
		usleep(5000);
		if (msecs == 1000) 
		{
			secs++;
			msecs = 0;
		}
		if (secs == 60) 
		{
			secs = 0;
			mins++;
		}
		if (mins == 60) 
		{
			mins = 0;
			hours++;
		}
		if (hours == 10) 
		{	
			hours = 0;
		}
	}
	printf("\a");
}

