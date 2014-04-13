#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "hcms.h"
#include "../SFE_SPI/SFE_SPI.h"
#include "../gpio/gpio.h"
#include "characters.h"

hcms::hcms(int numChars)
{
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
