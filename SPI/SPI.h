#ifndef spi_h 
#define spi_h

#include "../gpio/gpio.h"
#include <linux/spi/spidev.h>
#define SPIPIN '2'
class SPI
{
	public:
		SPI();
		SPI(unsigned char spiMode);
		SPI(unsigned char spiMode, long speed);
		SPI(unsigned char spiMode, long speed, bool lsbFirst);
		SPI(unsigned char spiMode, long speed, bool lsbFirst, gpio *csPin);
		~SPI();

		void writeBytes(char *buffer, int txLen);

	private:
		void init(unsigned char spiMode, long speed, bool lsbFirst, gpio *csPin);

		int _spiDev;   // Will be a file pointer to the SPI device.
		gpio *_csPin;  // Pointer to a pin object, if the user wants a non-standard
		               //  chip select for this device.
		struct spi_ioc_transfer _xfer;
		int _mode;
		int _bits;
		bool _lsbFirst;
		unsigned long _speed;
		static SPI *_activeDevice;
};

#endif
