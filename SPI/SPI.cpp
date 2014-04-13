#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "../gpio/gpio.h"
#include "SPI.h"

static const char *spi_name = "/dev/spidev0.0";

SPI::~SPI()
{
	close(_spiDev);
}

SPI::SPI()
{
	init(SPI_MODE_0, 0, 1000000, false); 
}

SPI::SPI(unsigned char spiMode)
{
	init(spiMode, 0, 1000000, false); 
}

SPI::SPI(unsigned char spiMode, gpio *csPin)
{
	init(spiMode, csPin, 1000000, false); 
}

SPI::SPI(unsigned char spiMode, gpio *csPin, long speed)
{
	init(spiMode, csPin, speed, false); 
}

SPI::SPI(unsigned char spiMode, gpio *csPin, long speed, bool lsbFirst)
{
	init(spiMode, csPin, speed, lsbFirst); 
}

void SPI::init(unsigned char spiMode, gpio *csPin, long speed, bool lsbFirst)
{
  // As usual, we begin the relationship by establishing a file object which
  //   points to the SPI device. _spiDev is a private class member type int
	//   used just for this purpose.
  _spiDev = open(spi_name, O_RDWR);

	// It's extremely important that _mode start at 0, as we'll assemble it by
	//  OR-ing constants.
	_mode = 0;

	// The first thing to do is check for an alternate chip select pin GPIO
	//  object. If that pointer is null, we need to set a bit in the MODE value.
	if (csPin != 0)
	{
		_mode |= SPI_NO_CS;
		_csPin = csPin;
	}

	// spiMode should be 0, 1, 2, or 3, defined by the SPI_MODE_x constants
	//  defined in the spidev.h file. 
  _mode |= spiMode;
  
	// The boolean lsbfirst is a rare case where the bit order should be 
	//  reversed during transmission; it corresponds to the SPI_LSB_FIRST
	//  constant in the spidev.h file.
 	if (lsbFirst) 
	{
		_mode |= SPI_LSB_FIRST;
	}

	// We've assembled the important bits of the 32-bit mode word. Let's push
	//  it to the device driver.
	ioctl(_spiDev, SPI_IOC_WR_MODE, &_mode);

  // Some devices may require more than 8 bits of data per transfer word. The
  //   SPI_IOC_WR_BITS_PER_WORD command allows you to change this.
  int bits_per_word = 8;
  ioctl(_spiDev, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
  
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
  _xfer.speed_hz = speed;
  _xfer.cs_change = 1;
  _xfer.bits_per_word = 8;
}

void SPI::writeBytes(char *buffer, int txLen)
{
	_xfer.tx_buf = (unsigned long)buffer;
	_xfer.len = txLen;
	ioctl(_spiDev, SPI_IOC_MESSAGE(1), &_xfer);
}

