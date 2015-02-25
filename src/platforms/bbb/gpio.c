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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "platform.h"
#include "gpio.h"

static volatile mraa_gpio_context gpio_swdio;
static volatile mraa_gpio_context gpio_swdclk;

int gpio_enable(uint8_t pin) 
{
	char* board_name = mraa_get_platform_name();
	uint8_t pin_libmraa = 0;

    printf("hello mraa\n Version: %s\n Running on %s\n", mraa_get_version(), board_name);
	
	// Adapt pins to libmraa numeration
	switch (pin)
	{
		case GPIO_SWDIO:
			pin_libmraa = 36;

			// init the desired pin with libmraa
			gpio_swdio = mraa_gpio_init(pin_libmraa);

			// use register map pin handling
			if (mraa_gpio_use_mmaped(gpio_swdio, 1) != MRAA_SUCCESS)
    		{   
       			printf("mmapped access to gpio %d is not supported, falling back to normal mode\n", pin);
    		} else {
    			printf("Pin GP%d == lm%d succesfully setup for mmap usage\n", GPIO_SWDIO, pin_libmraa);
    		} 
			break;

		case GPIO_SWDCLK:
			pin_libmraa = 48;

			// init the desired pin with libmraa
			gpio_swdclk = mraa_gpio_init(pin_libmraa);

			// use register map pin handling
			if (mraa_gpio_use_mmaped(gpio_swdclk, 1) != MRAA_SUCCESS)
    		{   
       			printf("mmapped access to gpio %d is not supported, falling back to normal mode\n", pin);
    		} else {
    			printf("Pin GP%d == lm%d succesfully setup for mmap usage\n", GPIO_SWDIO, pin_libmraa);
    		}
			break;
		default:
			printf("Weird pin init\n");
			break;
	}

    return 0;
}

int gpio_direction(uint8_t pin, bool output) 
{
	uint8_t val = output ? MRAA_GPIO_OUT : MRAA_GPIO_IN;

	switch (pin)
	{
		case GPIO_SWDIO:
			mraa_gpio_dir(gpio_swdio, val);
			break;
		
		case GPIO_SWDCLK:
			mraa_gpio_dir(gpio_swdclk, val);
			break;
		default:
			printf("Weird pin write\n");
			break;
	}

	return 0;
}

int gpio_set_value(uint8_t pin, bool value)
{
	uint8_t val = value ? 1 : 0;

	switch (pin)
	{
		case GPIO_SWDIO:
			mraa_gpio_write(gpio_swdio, val);
			break;
		
		case GPIO_SWDCLK:
			mraa_gpio_write(gpio_swdclk, val);
			break;
		default:
			printf("Weird pin write\n");
			break;
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
	uint8_t value = 0;
	switch (pin)
	{
		case GPIO_SWDIO:
			value = mraa_gpio_read(gpio_swdio);
			break;
		
		case GPIO_SWDCLK:
			value = mraa_gpio_read(gpio_swdclk);
			break;

		default:
			printf("Weird pin read\n");
			break;
	}

	return (value == 1);
}

static mraa_gpio_context gpio_init(mraa_gpio_context pin_libmraa)
{
	// init the desired pin with libmraa
	mraa_gpio_context gp = mraa_gpio_init(pin_libmraa);

	// use register map pin handling
	if (mraa_gpio_use_mmaped(gp, 1) != MRAA_SUCCESS)
    {   
    	printf("mmapped access to gpio with libmraa number ==  %d is not supported, falling back to normal mode\n", pin_libmraa);
    } else {
    	printf("Pin lm%d succesfully setup for mmap usage\n", pin_libmraa);
    }

    return gp;
}
