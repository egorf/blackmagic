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
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define PLATFORM_NAME "Intel Edison"
#define SYSFS_CLASS_GPIO "/sys/class/gpio"
#define SYSFS_PINMODE_PATH "/sys/kernel/debug/gpio_debug/gpio"
#define MAX_SIZE 64
#define MAX_MODE_SIZE 8

#define MMAP_PATH "/sys/devices/pci0000:00/0000:00:0c.0/resource0"

//MMAP
static uint8_t *mmap_reg = NULL;
static int mmap_fd = 0;
static int mmap_size;

extern int gpio_enable(uint8_t pin);
extern int gpio_direction(uint8_t pin, bool output);
extern int gpio_set_value(uint8_t pin, bool value);
extern int gpio_set(uint8_t pin);
extern int gpio_clear(uint8_t pin);
extern bool gpio_get(uint8_t pin);

#endif /* __GPIO_H */
