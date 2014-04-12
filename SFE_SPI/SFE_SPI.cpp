#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "SFE_SPI.h"

static const char *spi_name = "/dev/spidev0.0";

SFE_SPI::~SFE_SPI()
{
	close(_spiDev);
}

SFE_SPI::SFE_SPI(int spiMode)
{
  // The very first thing we need to do is make sure that the pins are set
  //   to SPI mode, rather than, say, GPIO mode.
  char path[256];
  
  for (int i = 10; i<=13; i++)
  {
    // Clear the path variable...
    memset(path,0,sizeof(path));
    // ...then assemble the path variable for the current pin mode file...
    sprintf(path, "%s%s%d", GPIO_MODE_PATH, GPIO_FILENAME, i);
		printf("%d", strlen(path));
    // ...and create a file descriptor...
    int pinMode = open(path, O_RDWR);
    // ...which we then use to set the pin mode to SPI...
    writeFile(pinMode, pinModeSPI);
    // ...and then, close the pinMode file.
    close(pinMode);
  }
    
  // As usual, we begin the relationship by establishing a file object which
  //   points to the SPI device. _spiDev is a private class member type int
	//   used just for this purpose.
  _spiDev = open(spi_name, O_RDWR);
  
  // We'll want to configure our SPI hardware before we do anything else. To do
  //   this, we use the ioctl() function. Calls to this function take the form
  //   of a file descriptor, a "command", and a value. The returned value is
  //   always the result of the operation; pass it a pointer to receive a value
  //   requested from the SPI peripheral.
  
  // Start by setting the mode. If we wanted to *get* the mode, we could
  //   use SPI_IOC_RD_MODE instead. In general, the "WR" can be replaced by
  //   "RD" to fetch rather than write. Also note the somewhat awkward
  //   setting a variable rather than passing the constant. *All* data sent
  //   via ioctl() must be passed by reference!
  int mode = spiMode;
  ioctl(_spiDev, SPI_IOC_WR_MODE, &mode);
  
  // In rare cases, you may find that a device expects data least significant
  //   bit first; in that case, you'll need to set that up. Writing a 0
  //   indicates MSb first; anything else indicates LSb first.
  int lsb_setting = 0;
  ioctl(_spiDev, SPI_IOC_WR_LSB_FIRST, &lsb_setting);
  
  // Some devices may require more than 8 bits of data per transfer word. The
  //   SPI_IOC_WR_BITS_PER_WORD command allows you to change this; the default,
  //   0, corresponds to 8 bits per word.
  int bits_per_word = 0;
  ioctl(_spiDev, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
  
  //    * tx_buf - a pointer to the data to be transferred
  //    * rx_buf - a pointer to storage for received data
  //    * len - length in bytes of tx and rx buffers
  //    * speed_hz - the clock speed, in Hz
  //    * delay_usecs - delay between last bit and deassertion of CS
  //    * bits_per_word - override global word length for this transfer
  //    * cs_change - strobe chip select between transfers?
  //    * pad - ??? leave it alone.
  
  
  memset(&_xfer, 0, sizeof(_xfer));
  _xfer.speed_hz = 50000;
  _xfer.cs_change = 1;
	_xfer.delay_usecs = 10;
  _xfer.bits_per_word = 8;
}

void SFE_SPI::SPIWriteBytes(char *buffer, int txLen)
{
	_xfer.tx_buf = (unsigned long)buffer;
	_xfer.len = txLen;
	ioctl(_spiDev, SPI_IOC_MESSAGE(1), &_xfer);
}

// While it seems okay to only *read* the first value from the file, you
//   seemingly must write four bytes to the file to get the I/O setting to
//   work properly. This function does that.
void SFE_SPI::writeFile(int fileID, int value)
{
  char buffer[4];  // A place to build our four-byte string.
  memset((void *)buffer, 0, sizeof(buffer)); // clear the buffer out.
  sprintf(buffer, "%d", value);
  lseek(fileID, 0, SEEK_SET);   // Make sure we're at the top of the file!
  write(fileID, buffer, sizeof(buffer));
}
