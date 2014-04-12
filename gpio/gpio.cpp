/***************************************************************************
gpio_test.cpp

Example code for GPIO access on the pcDuino via C++.

26 Mar 2013 - Mike Hord, SparkFun Electronics

This code is beerware- if you find it useful, please buy me (or, for that
matter, any other SparkFun employee you met) a pint next time you meet us at
the local.

***************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"

gpio::gpio(int pinID, int initMode)
{
  char path[256]; // nice, long buffer to hold the path name for pin access

	memset(path,0,sizeof(path));
	sprintf(path, "%s%s%d", GPIO_MODE_PATH, GPIO_FILENAME, pinID);
	_modeFileID = open(path, O_RDWR);
  writeFile(_modeFileID, initMode);

	sprintf(path, "%s%s%d", GPIO_PIN_PATH, GPIO_FILENAME, pinID);
  _pinFileID = open(path, O_RDWR);
}

gpio::~gpio()
{
	close(_modeFileID);
	close(_pinFileID);
}

// While it seems okay to only *read* the first value from the file, you
//   seemingly must write four bytes to the file to get the I/O setting to
//   work properly. This function does that.
void gpio::writeFile(int fileID, int value)
{
  char buffer[4];  // A place to build our four-byte string.
  memset((void *)buffer, 0, sizeof(buffer)); // clear the buffer out.
  sprintf(buffer, "%c", value);
  lseek(fileID, 0, SEEK_SET);   // Make sure we're at the top of the file!
  write(fileID, buffer, sizeof(buffer));
}

void gpio::mode(int newMode)
{
	writeFile(_modeFileID, newMode);
}

void gpio::pinWrite(int newLevel)
{
	writeFile(_pinFileID, newLevel);
}

int gpio::read()
{
	return 0;
}
