 /*
 * spi.h
 *
 * Modifications by Carlos Vargas, Universidad Nacional de Colombia
 *
 * Almost entirely based on Software by timtianyang:
 * Copyright (c) 2015, timtianyang
 * https://github.com/timtianyang/PathRevB/tree/master/bbb
 * All rights reserved.
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <linux/spi/spidev.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define uint8_t unsigned char

class SPI
{
	private:
	uint8_t mode;
	uint8_t bpw;
	bool lsb_first;
	uint32_t speed;
	bool active;
	int fd;

	public:

	SPI();
	int open(int bus, int channel);
	int close();
	int setMode(uint8_t mode);
	int setClockPolarity(uint8_t pol);
	int setClockPhase(uint8_t phase);
	int setLSBFirst(bool lsb_first);
	int setBitsPerWord(int bits);
	int setSpeed(uint32_t speed);
	int write(uint8_t wbuf[], int len);
	int read(uint8_t rbuf[], int len);
	int xfer1(uint8_t wbuf[], uint8_t rbuf[], int len);
	virtual ~SPI();
};

#endif /* SPI_H_ */
