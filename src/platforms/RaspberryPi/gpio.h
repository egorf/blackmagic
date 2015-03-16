/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2011  Black Sphere Technologies Ltd.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include <stdbool.h>
 #include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_SIZE_LINE 		64

#define LOW                 0
#define HIGH                1

// Raspberry Pi GPIO memory
#define BCM2708_PERI_BASE   0x20000000
#define BCM2709_PERI_BASE   0x3f200000
#define GPIO_BASE           (BCM2708_PERI_BASE + 0x200000)
#define GPIO_BASE_2			(BCM2709_PERI_BASE + 0x200000)
#define PAGE_SIZE           (4*1024)
#define BLOCK_SIZE          (4*1024)

// GPIO setup. Always use INP_GPIO(x) before OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define GPIO_MODE_IN(g)     *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define GPIO_MODE_OUT(g)    *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define GPIO_MODE_ALT(g,a)  *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))
#define GPIO_SET_HIGH       *(gpio+7)  // sets   bits which are 1
#define GPIO_SET_LOW        *(gpio+10) // clears bits which are 1
#define GPIO_GET(g)         (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

// Raspberry Pi GPIO mapping
#define RPI_GPIO_2    2    // Pin 3     SDA
#define RPI_GPIO_3    3    // Pin 5     SCL
#define RPI_GPIO_4    4    // Pin 7             NAVIO_PPM_INPUT
#define RPI_GPIO_7    7    // Pin 26    CE1     NAVIO_MPU9250_CS
#define RPI_GPIO_8    8    // Pin 24    CE0     NAVIO_UBLOX_CS
#define RPI_GPIO_9    9    // Pin 21    MISO
#define RPI_GPIO_10   10   // Pin 19    MOSI
#define RPI_GPIO_11   11   // Pin 23    SCLK
#define RPI_GPIO_14   14   // Pin 8     TxD
#define RPI_GPIO_15   15   // Pin 10    RxD
#define RPI_GPIO_17   17   // Pin 11            NAVIO_UART_PORT_5
#define RPI_GPIO_18   18   // Pin 12            NAVIO_UART_PORT_4
#define RPI_GPIO_22   22   // Pin 15            NAVIO_UBLOX_PPS
#define RPI_GPIO_23   23   // Pin 16            NAVIO_MPU9250_DRDY
#define RPI_GPIO_24   24   // Pin 18            NAVIO_SPI_PORT_6
#define RPI_GPIO_25   25   // Pin 22            NAVIO_SPI_PORT_5
#define RPI_GPIO_27   27   // Pin 13            NAVIO_PCA9685_OE
#define RPI_GPIO_28   28   // Pin 3
#define RPI_GPIO_29   29   // Pin 4
#define RPI_GPIO_30   30   // Pin 5
#define RPI_GPIO_31   31   // Pin 6

extern int gpio_enable(uint8_t pin);
extern int gpio_direction(uint8_t pin, bool output);
extern int gpio_set_value(uint8_t pin, bool value);
extern int gpio_set(uint8_t pin);
extern int gpio_clear(uint8_t pin);
extern bool gpio_get(uint8_t pin);
extern int getRaspberryPiVersion();

volatile uint32_t *gpio;
static int RPiVersion;

#endif /* __GPIO_H */
