 /*
 *gpio.cpp
 *
 * Modifications by Carlos Vargas, Universidad Nacional de Colombia
 *
 * Almost entirely based on Software by RidgeRun:
 * Copyright (c) 2011, RidgeRun
 * All rights reserved.
 */

#include "gpio.h"

gpio::gpio(unsigned int value)
{
    pin = value;
    polltiemout = 3000000000;
    nfds = 2;
    file = -1;
    configure(OUTPUT_PIN, NONE);
}

void gpio::configure( PIN_DIRECTION _direction, PIN_EDGE _edge)
{
    export_io();
    set_dir(_direction);
    set_edge(_edge);
    snprintf(p_value, sizeof(p_value), SYSFS_GPIO_DIR "gpio%d/value", pin);
    file = -1;
}

void gpio::export_io()
{
    char buf[MAX_BUF];
    int fd;
    int len;

    fd = open(SYSFS_GPIO_DIR "export", O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/export");
        return;
    }
    len = snprintf(buf, sizeof(buf), "%d", pin);
    int w = write(fd, buf, len);
    close(fd);
}

void gpio::unexport_io()
{
    int len;
    char buf[MAX_BUF];
    int fd;

    fd = open(SYSFS_GPIO_DIR "unexport", O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/export");
        return;
    }
    len = snprintf(buf, sizeof(buf), "%d", pin);
    int w = write(fd, buf, len);
    close(fd);
}

void gpio::set_dir(PIN_DIRECTION out_flag)
{
    int w;
    char buf[MAX_BUF];
    int fd;

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "gpio%d/direction", pin);
    fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/direction");
        return;
    }
    if (out_flag == OUTPUT_PIN)
        w = write(fd, "out", 4);
    else
        w = write(fd, "in", 3);
    close(fd);
}

void gpio::set(bool value)
{
    if (value)
        set(HIGH);
    else
        set(LOW);
}

void gpio::set(PIN_VALUE value)
{
    int w;
    bool toclose = false;
    if (file < 0)
    {
        toclose = true;
        file = open(p_value, O_WRONLY);
    }
    if (file < 0)
    {
        perror("gpio/set-value");
        return;
    }
    if (value == LOW)
        w = write(file, "0", 2);
    else
        w = write(file, "1", 2);
    if (toclose)
    {
        close(file);
        file = -1;
    }
}

int gpio::get()
{
    char ch;
    int val;
    int r;
    bool toclose = false;
    if (file < 0)
    {
        toclose = true;
        file = open(p_value, O_RDONLY);
    }
    if (file < 0)
    {
        perror("gpio/get-value");
        return -1;
    }
    r = read(file, &ch, 1);
    if (ch == '0')
    {
        val = 0;
    }
    else
    {
        val = 1;
    }
    if (toclose)
    {
        close(file);
        file = -1;
    }
    return val;
}

void gpio::set_edge(PIN_EDGE _edge)
{
    char buf[MAX_BUF];
    int fd;
    int w;

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", pin);
    fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/set-edge");
        return;
    }
    if (_edge == NONE)
        w = write(fd, "none", 5);
    else if (_edge == RISING)
        w = write(fd, "rising", 7);
    else if (_edge == FALLING)
        w = write(fd, "falling", 8);
    else if (_edge == BOTH)
        w = write(fd, "both", 5);
    close(fd);
}

int gpio::open_io()
{
    file = open(p_value, O_RDONLY | O_NONBLOCK);
    if (file < 0)
    {
        perror("gpio/fd_open");
    }
    return file;
}

void gpio::close_io()
{
    close(file);
    file = -1;
}

int gpio::get_interrupt()
{
    int len;
    int rc;
    int event = 0;
    bool toclose = false;

    if (file < 0)
    {
        toclose = true;
        file = open(p_value, O_WRONLY);
    }

    memset((void *)fdset, 0, sizeof(fdset));

    fdset[0].fd = STDIN_FILENO;
    fdset[0].events = POLLIN;

    fdset[1].fd = file;
    fdset[1].events = POLLPRI;

    rc = poll(fdset, nfds, polltiemout);

    if (rc < 0)
    {
        printf("\npoll() failed!\n");
        return -1;
    }

    if (rc == 0)
    {
        printf(".");
        event = 0;
    }

    if (fdset[1].revents & POLLPRI)
    {
        len = read(fdset[1].fd, p_value, MAX_BUF);
        event = 1;
        printf("\npoll() gpio %d interrupt occurred\n", pin);
    }

    if (fdset[0].revents & POLLIN)
    {
        int r = read(fdset[0].fd, p_value, 1);
        event = 3;
        printf("\npoll() stdin read 0x%2.2X\n", (unsigned int)p_value[0]);
    }

    fflush(stdout);

    if (toclose)
    {
        close(file);
        file = -1;
    }
    return event;
}

void gpio::set_timeout_interrupt(unsigned int timeout)
{
    polltiemout = timeout;
}

gpio::~gpio()
{
    unexport_io();
    close_io();
}