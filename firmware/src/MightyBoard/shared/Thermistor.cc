/*
 * Copyright 2010 by Adam Mayer	 <adam@makerbot.com>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "Thermistor.hh"
#include "ThermistorTable.hh"
#include "AnalogPin.hh"
//#include <util/atomic.h>

#include <algorithm>

extern "C" {
	#include "comm.h"
}

struct ThermTableEntry {
        int16_t adc;
        int16_t celsius;
} __attribute__ ((packed));


Thermistor::Thermistor(uint8_t analog_pin_in, uint8_t table_index_in) :
    analog_pin(analog_pin_in),
    next_sample(0),
    raw_valid(false),
    table_index(table_index_in)
{
	  if (analog_pin==0)
		  analog_pin = 25;
	  if (analog_pin==1)
		  analog_pin = 26;
        for (int i = 0; i < SAMPLE_COUNT; i++) {
            sample_buffer[i] = 0;
        }
}

void Thermistor::init() {
  current_temp = 0;
	initAnalogPin(analog_pin);
}

Thermistor::SensorState Thermistor::update() {
//	xprintf("Thermistor::SensorState Thermistor::update() %d" "\n",analog_pin);
	int16_t temp;
	bool valid;

//	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		valid = raw_valid;
		temp = raw_value;

		// Invalidate the result now that we have read it
		if (raw_valid) {
			raw_valid = false;
                }
//	}

	// initiate next read
	if (!startAnalogRead(analog_pin,&raw_value, &raw_valid)) return SS_ADC_BUSY;

	// If we haven't gotten data yet, return.
	if (!valid) return SS_ADC_WAITING;

	sample_buffer[next_sample] = temp;
	next_sample = (next_sample+1) % SAMPLE_COUNT;

	// average
//	int16_t cumulative = 0;
//	for (int i = 0; i < SAMPLE_COUNT; i++) {
//		cumulative += sample_buffer[i];
//	}

	// my filter sorting
	uint32_t sort_table[SAMPLE_COUNT];
	sample_buffer[next_sample]=raw_value;
	for (uint8_t i=0;i<SAMPLE_COUNT;i++)
		sort_table[i]=sample_buffer[i];
	std::sort(sort_table, sort_table+SAMPLE_COUNT);
	temp = sort_table[4];

//	for (uint8_t i=0;i<9;i++){
//		xprintf("i:%d sample_buffer[%d]:%d\n",i,i,sample_buffer[i]);
//	}

	// TODO: The raw_value appears to be 0 the first time this loop is run,
	//       which causes this failsafe to trigger unnecessarily. Disabling
	//       for now, since it doesn't work for ABP/HBP thermistors.
	if ((temp > ADC_RANGE - 2) || (temp < 2)) {
                current_temp = BAD_TEMPERATURE;	// Set the temperature to 1024 as an error condition
		return SS_ERROR_UNPLUGGED;
	}

//	int16_t avg = cumulative / SAMPLE_COUNT;

	//current_temp = thermistorToCelsius(avg,table_index);
	current_temp = thermistorToCelsius(temp,table_index);

//	xprintf("current_temp:%d\n",temp);

	return SS_OK;
}
