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

int getRaspberryPiVersion()
{
    char buffer[MAX_SIZE_LINE];
    char* c = "Hardware";
    char* v1 = "BCM2708";
    char* v2 = "BCM2709";
    char* flag;
    FILE* fd;

    fd = fopen("/proc/cpuinfo", "r");

    while(fgets(buffer, MAX_SIZE_LINE, fd) != NULL)
    {
        flag = strstr(buffer, c);

        if (flag != NULL) 
        {
            if (strstr(buffer, v2) != NULL)
            {
                printf("Raspberry Pi 2 with BCM2709!\n");
                fclose(fd);
                return 2;
            } 
            else if (strstr(buffer, v1) != NULL) 
            {
                printf("Raspberry Pi 1 with BCM2708!\n");
                fclose(fd);
                return 1;
            }
        }
    }

    // defaults to 1!!!!!
    printf("Could not detect RPi version, defaulting to 1\n");
    fclose(fd);
    return 1;
}

int gpio_enable(uint8_t pin) 
{
    int mem_fd;
    void* gpio_map;
    int gp_base = GPIO_BASE;

    if(RPiVersion == 2)
        gp_base = GPIO_BASE_2;

    // open /dev/mem
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
    {
        printf("Cant open /dev/mem \n");
        exit(-1);
    }

    // mmap GPIO
    gpio_map = mmap
    (
        NULL,                 // Any adddress in our space will do
        BLOCK_SIZE,           // Map length
        PROT_READ|PROT_WRITE, // Enable reading & writting to mapped memory
        MAP_SHARED,           // Shared with other processes
        mem_fd,               // File to map
        gp_base            // Offset to GPIO peripheral
    );

    close(mem_fd); //No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED) 
    {
        printf("mmap error %d\n", (int)gpio_map);//errno also set!
        exit(-1);
    }

    gpio = (volatile uint32_t *)gpio_map; // Always use volatile pointer!

    return 0;
}

int gpio_direction(uint8_t pin, bool output) 
{
    if (output == 0) 
    {
        GPIO_MODE_IN(pin);
    } else {
        GPIO_MODE_IN(pin);
        GPIO_MODE_OUT(pin);
    }

    return 0;
}

int gpio_set_value(uint8_t pin, bool value) 
{
    if (value == LOW) {
        GPIO_SET_LOW = 1 << pin;
    } else {
        GPIO_SET_HIGH = 1 << pin;
    }

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
    uint32_t value = GPIO_GET(pin);
    return value ? 1: 0;
}


