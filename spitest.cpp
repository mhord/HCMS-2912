#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "SPI/SPI.h"
#include "gpio/gpio.h"


int main()
{
	printf("Hello!\n\r");
	SPI spiTester;
	char test[] = "12345";
	spiTester.writeBytes(test, strlen(test));
	SPI spiTester1(SPI_MODE_3);
	spiTester1.writeBytes(test, strlen(test));
	printf("\a");
}

