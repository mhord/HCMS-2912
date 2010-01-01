#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "../gpio/gpio.h"
#include "spi_device.h"
#include "spi_port.h"

spi_device *_portOwner = NULL;

spi_port::~spi_port()
{
	close(_spiFile);
}

spi_port::spi_port(int spiFile, int spiDriver)
									 
{
	_spiFile = spiFile;
	configurePins(spiDriver);
}

void spi_port::configurePins(int spiDriver)
{
	if (spiDriver == 0)
	{
		_MOSI = new gpio(11, SPIPIN);
		_MISO = new gpio(12, SPIPIN);
		_SCK = new gpio(13, SPIPIN);
		_CS = new gpio (10, SPIPIN, HIGH);
	}
	else if (spiDriver == 2)
	{
		_MOSI = new gpio(21, SPIPIN);
		_MISO = new gpio(22, SPIPIN);
		_SCK = new gpio(23, SPIPIN);
		_CS = new gpio(20, SPIPIN, HIGH);
	}
}

// configurePort() only handles the parts which must be handled apart from
//  the message struct.
void spi_port::configurePort()
{
	if (_portOwner == NULL)
	{
		return;
	}

	// Set up the CS pin for the operation. We'll (naively) assume that the user
	//  hasn't done anything to the mode of the pins since we made them SPI pins,
	//  but we *do* need to worry about CS. If the owner of the port has a 
	//  non-standard CS pin, we need to use that one. Otherwise, we should use
	//  the default. Note that the NO_CS_PIN argument in the spi_ioc_transfer
	//  struct doesn't work with this library.
	if (_portOwner->_csPin == NULL)
	{
		_CS->mode(SPIPIN);
	}
	else
	{
		_CS = _portOwner->_csPin;
	}

	// We'll make a little temp variable to transmit the various mode info to 
	//  the SPI driver.
	int temp = 0;
  
	// The boolean lsbfirst is a rare case where the bit order should be 
	//  reversed during transmission; it corresponds to the SPI_LSB_FIRST
	//  constant in the spidev.h file.
 	if (_portOwner->_lsbFirst) 
	{
		temp = SPI_LSB_FIRST;
		ioctl(_spiFile, SPI_IOC_WR_LSB_FIRST, &temp);
	}
	else
	{
		ioctl(_spiFile, SPI_IOC_WR_LSB_FIRST, &temp);
	}

	// spiMode should be 0, 1, 2, or 3, defined by the SPI_MODE_x constants
	//  defined in the spidev.h file. 
	temp = _portOwner->_spiMode ;     // Store in temp for immediate use.
  
	// We've assembled the stuff that needs to be sent to the "mode" setting;
	//  send it along.
	ioctl(_spiFile, SPI_IOC_WR_MODE, &temp);
}

void spi_port::transferData(spi_ioc_transfer *xfer)
{
	if (_portOwner->_csPin != NULL)
	{
		(_portOwner->_csPin->pinWrite(LOW));
	}
	ioctl(_spiFile, SPI_IOC_MESSAGE(1), xfer);
	if (_portOwner->_csPin != NULL)
	{
		(_portOwner->_csPin->pinWrite(HIGH));
	}
}

