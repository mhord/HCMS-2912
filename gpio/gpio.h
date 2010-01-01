#ifndef __gpio_test_h__
#define __gpio_test_h__


// Make some aliases to make the code more readable.
#define HIGH '1'
#define LOW  '0'

#define INPUT  '0'
#define OUTPUT '1'
#define INPUT_PU '8'
#define SPIPIN '2'

class gpio
{
	public:
		gpio(int pinID, int initMode);
		~gpio();
		void mode(int newMode);
		void pinWrite(int newLevel);
		int read();

	private:
		void writeFile(int fileID, int value);
		int _modeFileID;
		int _pinFileID;
};

#endif

