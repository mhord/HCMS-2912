#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "../gpio/gpio.h"
#include "spi_device.h"
#include "spi_port.h"

spi_device::spi_device(spi_port *port, \
					 unsigned char spiMode, \
					 long speed, \
					 bool lsbFirst, \
					 gpio *csPin)
{
		_port = port;
		_spiMode = spiMode;
		_speed = speed;
		_lsbFirst = lsbFirst;
		_csPin = csPin;
}

void spi_device::transferData(unsigned char *dataOut, \
                         unsigned char *dataIn, \
												 unsigned int len,
												 bool deselect)
{
	spi_ioc_transfer xfer;	
	xfer.tx_buf = (unsigned long)dataOut;
	xfer.rx_buf = (unsigned long)dataIn;
	xfer.len = len;
	xfer.speed_hz = _speed;
	xfer.bits_per_word = 8;
	if (deselect)
	{
		xfer.cs_change = 1;
	}
	else
	{
		xfer.cs_change = 0;
	}

	if (_port->_portOwner != this)
	{
		_port->_portOwner = this;
		_port->configurePort();
	}

	_port->transferData(&xfer);
}
