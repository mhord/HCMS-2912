#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "../gpio/gpio.h"
#include "SPI.h"

static const char *spi_name = "/dev/spidev0.0";

SPI *_activeDevice=NULL;

SPI::~SPI()
{
	close(_spiDev);
}

SPI::SPI()
{
	init(SPI_MODE_0, 1000000, false, NULL); 
}

SPI::SPI(unsigned char spiMode)
{
	init(spiMode, 1000000, false, NULL); 
}

SPI::SPI(unsigned char spiMode, long speed)
{
	init(spiMode, speed, false, NULL); 
}

SPI::SPI(unsigned char spiMode, long speed, bool lsbFirst )
{
	init(spiMode, speed, false, NULL); 
}

SPI::SPI(unsigned char spiMode, long speed, bool lsbFirst, gpio *csPin)
{
	init(spiMode, speed, lsbFirst, csPin); 
}

void SPI::init(unsigned char spiMode, long speed, bool lsbFirst, gpio *csPin)
{
	// Grab the input parameters and stuff them in the local variables.
	_speed = speed;
	_csPin = csPin;
	_lsbFirst = lsbFirst;
	_mode = spiMode;

	// First things first: let's make sure our pins are all in SPI mode.
	gpio SPI_MOSI(11, SPIPIN);
	gpio SPI_MISO(12, SPIPIN);
	gpio SPI_CLK(13, SPIPIN);

	if (_csPin == NULL)
	{
		gpio SPI_CS(10, SPIPIN);
	}
	
  // As usual, we begin the relationship by establishing a file object which
  //   points to the SPI device. _spiDev is a private class member type int
	//   used just for this purpose.
  _spiDev = open(spi_name, O_RDWR);

	// We'll make a little temp variable to transmit these various things to the
	//  SPI driver.
	int temp = 0;

	// The boolean lsbfirst is a rare case where the bit order should be 
	//  reversed during transmission; it corresponds to the SPI_LSB_FIRST
	//  constant in the spidev.h file.
 	if (_lsbFirst) 
	{
		temp = SPI_LSB_FIRST;
		ioctl(_spiDev, SPI_IOC_WR_LSB_FIRST, &temp);
	}
	else
	{
		ioctl(_spiDev, SPI_IOC_WR_LSB_FIRST, &temp);
	}

	// spiMode should be 0, 1, 2, or 3, defined by the SPI_MODE_x constants
	//  defined in the spidev.h file. 
  _mode = spiMode;  // Store setting permanently.
	temp = _mode;     // Store in temp for immediate use.
  
	// The last element of _mode that we worry about is whether we should set
	//  the SPI_NO_CS flag. If we're using an alternate CS pin, we don't want
	//  to accidentally trigger the native CS pin when we transmit.
	if (_csPin != 0)
	{
		temp |= SPI_NO_CS;
	}
	
	// We've assembled the stuff that needs to be sent to the "mode" setting;
	//  send it along.
	ioctl(_spiDev, SPI_IOC_WR_MODE, &temp);

  // Some devices may require more than 8 bits of data per transfer word. The
  //   SPI_IOC_WR_BITS_PER_WORD command allows you to change this. Oddly, this
	//   seems to be '0' for the default 8-bit setting.
  int bits_per_word = 0;
  ioctl(_spiDev, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
  
	// We want to cache the speed value for later, in case we need to restore
	//  the settings if they get changed by some other object
	_speed = speed;

	// The _xfer struct will be used for each transfer; we'll set some defaults
	//  here as well.
  //    * tx_buf - a pointer to the data to be transferred
  //    * rx_buf - a pointer to storage for received data
  //    * len - length in bytes of tx and rx buffers
  //    * speed_hz - the clock speed, in Hz
  //    * delay_usecs - delay between last bit and deassertion of CS
  //    * bits_per_word - override global word length for this transfer
  //    * cs_change - strobe chip select between transfers?
  //    * pad - ??? leave it alone.
  
  memset(&_xfer, 0, sizeof(_xfer));
  _xfer.speed_hz = _speed;
  _xfer.cs_change = 1;
  _xfer.bits_per_word = 8;
	_xfer.pad = 0;

//	_activeDevice = this;
}

void SPI::writeBytes(char *buffer, int txLen)
{
/*	if (_activeDevice != this)
	{
		_activeDevice = this;

		memset(&_xfer, 0, sizeof(_xfer));
		_xfer.speed_hz = _speed;
		_xfer.cs_change = 1;
		_xfer.bits_per_word = 8;
		
		int bits_per_word = 8;
		ioctl(_spiDev, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);

		ioctl(_spiDev, SPI_IOC_WR_MODE, &_mode);
	}*/
	_xfer.tx_buf = (unsigned long)buffer;
	_xfer.len = txLen;
	if (_csPin != 0)
	{
		(_csPin->pinWrite(LOW));
	}
	ioctl(_spiDev, SPI_IOC_MESSAGE(1), &_xfer);
	if (_csPin != 0)
	{
		(_csPin->pinWrite(HIGH));
	}
}

