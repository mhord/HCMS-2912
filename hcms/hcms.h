#ifndef __hcms_h_
#define __hcms_h_

#include "../SPI/SPI.h"
#include "../gpio/gpio.h"

class hcms : public SPI
{
	public:
		hcms(int numChars, gpio *resetPin,\
		           gpio *rsPin, gpio *blankPin, gpio *chipSelect=NULL);
		~hcms();
		void initDisplay();
		void clearDisplay();
		void printChar(char printee);
		void blank();
		void unblank();
	private:
		int _numChars;
		gpio *_resetPin;
		gpio *_blankPin;
		gpio *_rsPin;
		spi_device *_myDevice;
};

#endif

