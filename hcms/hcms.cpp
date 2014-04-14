#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>

#include "../gpio/gpio.h"
#include "../SPI/SPI.h"
#include "characters.h"
#include "hcms.h"

SPI *SPI::_activeDevice;

hcms::~hcms()
{
	close(_spiDev);
}

hcms::hcms(int numChars, gpio *resetPin,\
		       gpio *rsPin, gpio *blankPin)
{
	_spiDev = open("/dev/spidev0.0", O_RDWR);
	_numChars = numChars;
	_resetPin = resetPin;
	_rsPin = rsPin;
	_blankPin = blankPin;
	_activeDevice = this;
	_mode = SPI_MODE_0;
	_speed = 3000000;
	init(_mode, 0, _speed, false);
}

hcms::hcms(int numChars, gpio *resetPin,\
		       gpio *rsPin, gpio *blankPin, gpio *chipSelect)
{
	_spiDev = open("/dev/spidev0.0", O_RDWR);
	_numChars = numChars;
	_resetPin = resetPin;
	_rsPin = rsPin;
	_blankPin = blankPin;

	_activeDevice = this;
	_mode = SPI_MODE_0;
	_speed = 3000000;
	init(_mode, chipSelect, _speed, false);
}

void hcms::printChar(char printee)
{
	printee -= ' ';
	writeBytes(&characterArray[(unsigned char)printee][0], 5);
}

void hcms::initDisplay()
{
	_resetPin->pinWrite(LOW);  // Reset the config registers

	_rsPin->pinWrite(HIGH);    // Enable writing to config registers

	_resetPin->pinWrite(HIGH); // Release device(s) from reset.
	
  char cmdBuffer = 0x7F;// 0x7F is a good value to start for the displays	

	for (int i = 0; i < _numChars/4; i++	)
	{
		writeBytes(&cmdBuffer, 1);
	}
	_rsPin->pinWrite(LOW);
}

void hcms::clearDisplay()
{
	_blankPin->pinWrite(HIGH);
	for (int i=0; i<_numChars; i++)
	{
		printChar(' ');
	}
	_blankPin->pinWrite(LOW);
}

void blank()
{
	_blankPin->pinWrite(HIGH);
}

void unblank();
{
	_blankPin->pinWrite(LOW);
}
