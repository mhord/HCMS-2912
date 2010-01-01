#ifndef _spi_device_h
#define _spi_device_h

#include <linux/spi/spidev.h>

class spi_port; // Forward declaration for spi_port class.
class gpio;     // FOrward declaration for gpio class.

class spi_device
{
	friend class spi_port;
	public:
		spi_device(spi_port *port, \
		           unsigned char spiMode = SPI_MODE_0, \
							 long speed = 1000000, \
							 bool lsbFirst = false, \
							 gpio *csPin = NULL);
		void transferData(unsigned char *outData, \
		                  unsigned char *inData = NULL, \
											unsigned int len = 1, \
											bool deselect = true);
	protected:
		spi_port *_port;
		unsigned char _spiMode;
		long _speed;
		bool _lsbFirst;
		gpio *_csPin;

};
#endif

