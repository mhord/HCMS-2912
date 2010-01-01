#ifndef _spi_port_h 
#define _spi_port_h

class spi_device; // Forward declaration for spi_device class.
class gpio;       // Forward declaration for gpio class.
struct spi_ioc_transfer;

class spi_port
{
	friend class spi_device;

	public:
		spi_port(int spiFile, int spiDriver=1);
		~spi_port();

	private:
		void configurePort();
		void configurePins(int spiDriver); 
		void transferData(spi_ioc_transfer *xfer);

		int _spiFile;   // Will be a file pointer to the SPI device.
		spi_device *_portOwner;
		gpio *_MOSI;
		gpio *_MISO;
		gpio *_SCK;
		gpio *_CS;
};

#endif

