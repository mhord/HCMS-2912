#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hcms.h"
#include "SFE_SPI/SFE_SPI.h"
#include "gpio/gpio.h"
#include "characters.h"

#define RS_PIN 2
#define RESET_PIN 4
#define BLANK_PIN 3
#define LED_PIN 7

gpio RS(RS_PIN, OUTPUT);
gpio RESET(RESET_PIN, OUTPUT);
gpio BLANK(BLANK_PIN, OUTPUT);

SFE_SPI display(SPI_MODE0);

int main()
{
	initDisplay(24);        
	clearDisplay(24);
	char str1[] = "OMG WTF BBQ";
	for (int i=0; i<strlen(str1); i++)
	{
		printChar(str1[i]);
		usleep(100000);
	}
	for (int i=0; i<24-strlen(str1); i++)
	{
		printChar(' ');
		usleep(100000);
	}
	while(1);
	{
	}
	printf("\a");
}

void printChar(char printee)
{
	printee -= ' ';
	display.SPIWriteBytes(characterArray[printee], 5);
}

void initDisplay(int numChars)
{
	RESET.pinWrite(LOW);  // Reset the config registers
	RS.pinWrite(HIGH);    // Enable writing to config registers

	RESET.pinWrite(HIGH); // Release device(s) from reset.
	
  char cmdBuffer = 0x7F;// 0x7F is a good value to start for the displays	

	for (int i = 0; i < numChars/4; i++	)
	{
		display.SPIWriteBytes(&cmdBuffer, 1);
	}
	RS.pinWrite(LOW);
}

void clearDisplay(int numChars)
{
	BLANK.pinWrite(HIGH);
	for (int i=0; i<numChars; i++)
	{
		printChar(' ');
	}
	BLANK.pinWrite(LOW);
}
