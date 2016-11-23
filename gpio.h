/*
 *gpio.h
 *
 * Modifications by Carlos Vargas, Universidad Nacional de Colombia
 *
 * Almost entirely based on Software by RidgeRun:
 * Copyright (c) 2011, RidgeRun
 * All rights reserved.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define SYSFS_GPIO_DIR "/sys/class/gpio/"
#define MAX_BUF 64

enum PIN_DIRECTION
{
    INPUT_PIN = 0,
    OUTPUT_PIN = 1
};

enum PIN_VALUE
{
    LOW = 0,
    HIGH = 1
};

enum PIN_EDGE
{
    NONE = 0,
    RISING = 1,
    FALLING = 2,
    BOTH = 3
};

class gpio
{
  private:
    unsigned int pin;
    int polltiemout;
    char p_value[MAX_BUF];
    int file;
    int nfds;
    struct pollfd fdset[2];
    void export_io();
    void unexport_io();
    double value_;

  public:
    gpio(unsigned int value);
    virtual ~gpio();
    
    void configure(PIN_DIRECTION _direction, PIN_EDGE _edge);
    void set_dir(PIN_DIRECTION out_flag);
    void set_edge(PIN_EDGE _edge);
    void set(PIN_VALUE value);
    void set(bool value);
    void set_timeout_interrupt(unsigned int timeout);
    int get();
    int get_interrupt();
    int open_io();
    void close_io();
};
#endif /* SIMPLEGPIO_H_ */
