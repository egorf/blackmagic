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

/* This file implements the low-level SW-DP interface. */

#include <stdio.h>

#include "platform.h"
#include "general.h"
#include "swdptap.h"
#include "gdb_packet.h"
#include "gpio.h"

 static bool dbg = false;

int swdptap_init(void)
{
	/* This must be investigated in more detail.
	 * As described in STM32 Reference Manual... */
	swdptap_reset();
	swdptap_seq_out(0xE79E, 16); /* 0b0111100111100111 */
	swdptap_reset();
	swdptap_seq_out(0, 16);

	return 0;
}

static void swdptap_clock() {
	gpio_set(GPIO_SWDCLK);
	gpio_clear(GPIO_SWDCLK);
}

static void swdptap_turnaround(uint8_t dir)
{
	static uint8_t olddir = 0;

	if (dbg)
		DEBUG("%s", dir ? "\n-> ":"\n<- ");

	/* Don't turnaround if direction not changing */
	if(dir == olddir) return;
	olddir = dir;

	if(dir)
		SWDIO_MODE_FLOAT();
	swdptap_clock();
	if(!dir)
		SWDIO_MODE_DRIVE();
}

static uint8_t swdptap_bit_in(void)
{
	uint16_t ret;

	ret = gpio_get(GPIO_SWDIO);
	swdptap_clock();

	if(dbg)
		DEBUG("%d", ret?1:0);

	return ret != 0;
}

static void swdptap_bit_out(uint8_t val)
{
	if(dbg)
		DEBUG("%d", val);

	gpio_set_value(GPIO_SWDIO, val);
	swdptap_clock();
}


void swdptap_reset(void)
{
	swdptap_turnaround(0);
	/* 50 clocks with TMS high */
	for(int i = 0; i < 50; i++) swdptap_bit_out(1);
}


uint32_t swdptap_seq_in(int ticks)
{
	uint32_t index = 1;
	uint32_t ret = 0;

	swdptap_turnaround(1);

	while(ticks--) {
		if(swdptap_bit_in()) ret |= index;
		index <<= 1;
	}

	return ret;
}


uint8_t swdptap_seq_in_parity(uint32_t *ret, int ticks)
{
	uint32_t index = 1;
	uint8_t parity = 0;
	*ret = 0;

	swdptap_turnaround(1);

	while(ticks--) {
		if(swdptap_bit_in()) {
			*ret |= index;
			parity ^= 1;
		}
		index <<= 1;
	}
	if(swdptap_bit_in()) parity ^= 1;

	return parity;
}


void swdptap_seq_out(uint32_t MS, int ticks)
{
	swdptap_turnaround(0);

	while(ticks--) {
		swdptap_bit_out(MS & 1);
		MS >>= 1;
	}
}


void swdptap_seq_out_parity(uint32_t MS, int ticks)
{
	uint8_t parity = 0;

	swdptap_turnaround(0);

	while(ticks--) {
		swdptap_bit_out(MS & 1);
		parity ^= MS;
		MS >>= 1;
	}
	swdptap_bit_out(parity & 1);
}
