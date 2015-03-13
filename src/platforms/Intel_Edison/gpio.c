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



#include "platform.h"
#include "gpio.h"


int gpio_enable(uint8_t pin) 
{
    //Might need to make some elements of this thread safe.
    //For example only allow one thread to enter the following block
    //to prevent mmap'ing twice

    if (mmap_reg == NULL) {
        if ((mmap_fd = open(MMAP_PATH, O_RDWR)) < 0) 
        {
            printf("edison map: unable to open resource file\n");
            return -1;
        }

        struct stat fd_stat;
        fstat(mmap_fd, &fd_stat);
        mmap_size = fd_stat.st_size;

        mmap_reg = (uint8_t*) mmap(NULL, fd_stat.st_size,
                                   PROT_READ | PROT_WRITE,
                                   MAP_FILE | MAP_SHARED,
                                   mmap_fd, 0);
        
        if (mmap_reg == MAP_FAILED) 
        {
            printf("edison mmap: failed to mmap\n");
            mmap_reg = NULL;
            close(mmap_fd);
            return -2;
        }
    }

    return 0;
}

int gpio_direction(uint8_t pin, bool output) 
{
    // need to use SysFS
    char value[3];
    char filepath[MAX_SIZE];
    int length;
    int fd;

    sprintf(filepath, SYSFS_PINMODE_PATH "%d/current_direction", pin);

    fd = open(filepath, O_WRONLY);

    if (fd < 0) 
    {
        printf("GPIO resource unavailable!\n");
        return -1;
    }

    if (output)
    {
        length = sprintf(value, "out");
    } else {
        length = sprintf(value, "in");
    }

    if (write(fd, value, length) < 0 ) 
    {
        printf("Unable to write direction for the %d pin\n", pin);
        return -2;
    }

    close(fd);

    return 0;
}

int gpio_set_value(uint8_t pin, bool value)
{
    uint8_t offset = ((pin / 32) * sizeof(uint32_t));
    uint8_t valoff;

    if (value) {
        valoff = 0x34;
    } else {
        valoff = 0x4c;
    }

    *(volatile uint32_t*) (mmap_reg + offset + valoff) =
        (uint32_t)(1 << (pin % 32));

    return 0;
}

int gpio_set(uint8_t pin) 
{
	return gpio_set_value(pin, true);
}

int gpio_clear(uint8_t pin) 
{
	return gpio_set_value(pin, false);
}

bool gpio_get(uint8_t pin) 
{
    uint8_t offset = ((pin / 32) * sizeof(uint32_t));
    uint32_t value;

    value = *(volatile uint32_t*) (mmap_reg + 0x04 + offset);
    
    if (value&(uint32_t)(1 << (pin % 32))) {
        return true;
    }

    return false;
}

