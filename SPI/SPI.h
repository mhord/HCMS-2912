#ifndef spi_h 
#define spi_h

#define pinModeSPI 2 
#define GPIO_MODE_PATH "/sys/devices/virtual/misc/gpio/mode/"
#define GPIO_FILENAME  "gpio"

class SPI
{
	public:
		SPI();
		SPI(unsigned char spiMode);
		SPI(unsigned char spiMode, gpio *csPin);
		SPI(unsigned char spiMode, gpio *csPin, long speed);
		SPI(unsigned char spiMode, gpio *csPin, long speed, bool lsbFirst);
		~SPI();

		void writeBytes(char *buffer, int txLen);

	protected:
		void init(unsigned char spiMode, gpio *csPin, long speed, bool lsbFirst);

		int _spiDev;   // Will be a file pointer to the SPI device.
		gpio *_csPin;  // Pointer to a pin object, if the user wants a non-standard
		               //  chip select for this device.
		struct spi_ioc_transfer _xfer;
		unsigned long _mode;
		unsigned char _bits;
		unsigned long _speed;
};

#endif