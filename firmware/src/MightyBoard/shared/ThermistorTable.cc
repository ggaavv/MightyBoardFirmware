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

#include "ThermistorTable.hh"
#include "Configuration.hh"
#include "EepromMap.hh"
//#include <avr/eeprom.h>
#include <stdint.h>
//#include <avr/pgmspace.h>

extern "C" {
	#include "comm.h"
}

// TODO: Clean this up...
#if defined HAS_THERMISTOR_TABLES


// Default thermistor table.  If no thermistor table is loaded into eeprom,
// this will be copied in by the initTable() method.
//
// Thermistor lookup table for RepRap Temperature Sensor Boards (http://make.rrrf.org/ts)
// Made with createTemperatureLookup.py (http://svn.reprap.org/trunk/reprap/firmware/Arduino/utilities/createTemperatureLookup.py)
// ./createTemperatureLookup.py --r0=100000 --t0=25 --r1=0 --r2=4700 --beta=4066 --max-adc=1023
// r0: 100000
// t0: 25
// r1: 0
// r2: 4700
// beta: 4066
// max adc: 1023

/*TempTable const default_therm_table = {
  {1, 841},
  {54, 255},
  {107, 209},
  {160, 184},
  {213, 166},
  {266, 153},
  {319, 142},
  {372, 132},
  {425, 124},
  {478, 116},
  {531, 108},
  {584, 101},
  {637, 93},
  {690, 86},
  {743, 78},
  {796, 70},
  {849, 61},
  {902, 50},
  {955, 34},
  {1008, 3}
};*/
TempTable const default_therm_table = {
  /* {ADC value Extruder0, temperature} */
		{	105	,	290	}	,
		{	121	,	280	}	,
		{	140	,	270	}	,
		{	162	,	260	}	,
		{	189	,	250	}	,
		{	222	,	240	}	,
		{	261	,	230	}	,
		{	308	,	220	}	,
		{	365	,	210	}	,
		{	434	,	200	}	,
		{	519	,	190	}	,
		{	621	,	180	}	,
		{	744	,	170	}	,
		{	891	,	160	}	,
		{	1067	,	150	}	,
		{	1272	,	140	}	,
		{	1507	,	130	}	,
		{	1771	,	120	}	,
		{	2058	,	110	}	,
		{	2357	,	100	}	,
		{	2657	,	90	}	,
		{	2943	,	80	}	,
		{	3204	,	70	}	,
		{	3429	,	60	}	,
		{	3614	,	50	}	,
		{	3760	,	40	}	,
		{	3869	,	30	}	,
		{	3948	,	20	}	,
		{	4003	,	10	}	,
		{	4039	,	0	}
};

bool has_table[2];//TODO: using just a single bool for MightyBoard

inline Entry getEntry(int8_t entryIdx, int8_t which) {
//	xprintf("%d" " (%s:%d)\n",entryIdx,_F_,_L_);
	Entry rv;
	if (0){//has_table[which]) {
		// get from eeprom
		uint16_t offset = eeprom_offsets::THERM_TABLE + therm_eeprom_offsets::THERM_DATA_OFFSET;
/// for MightBoard just use one thermistor table
//		if (which == 0) {
//			offset = eeprom::THERM_TABLE_0 + eeprom::THERM_DATA_OFFSET;
//		}
//		else {
//			offset = eeprom::THERM_TABLE_1 + eeprom::THERM_DATA_OFFSET;
//		}
//		offset += sizeof(Entry) * entryIdx;				//TODO: Fix if if(0) changes
//		eeprom_read_block(&rv,(const void*)offset,sizeof(Entry)); //TODO: Fix if if(0) changes
	} else {
		// get from progmem
		rv.adc = default_therm_table[entryIdx].adc;
		rv.value = default_therm_table[entryIdx].value;
//		xprintf("default_therm_table:%d\n",entryIdx);
//		xprintf("adc:%d\n",rv.adc);
//		xprintf("value:%d\n",rv.value);
//		xprintf("%d" " (%s:%d)\n",rv.adc,_F_,_L_);
//		xprintf("%d" " (%s:%d)\n",rv.value,_F_,_L_);
//		memcpy_P(&rv, (const void*)&(default_therm_table[entryIdx]), sizeof(Entry));
	}
	return rv;
}

int16_t thermistorToCelsius(int16_t reading, int8_t table_idx) {
  int8_t bottom = 0;
  int8_t top = NUMTEMPS-1;
  int8_t mid = (bottom+top)/2;
  int8_t t;
  Entry e;
  while (mid > bottom) {
	  t = mid;
	  e = getEntry(mid,table_idx);
	  if (reading < e.adc) {
		  top = mid;
		  mid = (bottom+top)/2;
	  } else {
		  bottom = mid;
		  mid = (bottom+top)/2;
	  }
  }
  Entry eb = getEntry(bottom,table_idx);
  Entry et = getEntry(top,table_idx);
//	xprintf("default_therm_table\n\n\n");
//	xprintf("eb:%d\n",eb.value);
//	xprintf("et:%d\n",et.value);
  if (bottom == 0 && reading < eb.adc) {
	  // out of scale; safety mode
	  return 255;
  }
  if (top == NUMTEMPS-1 && reading > et.adc) {
	  // out of scale; safety mode
	  return 255;
  }

  int16_t celsius  = eb.value +
		  ((reading - eb.adc) * (et.value - eb.value)) / (et.adc - eb.adc);
  if (celsius > 255)
	  celsius = 255;
  return celsius;
}

bool isTableSet(uint16_t off) {
//	const void* offset = (const void*)off;
	uint8_t first_byte;
//	first_byte = (uint8_t)(eeprom_address(EEPROM_START_ADDRESS, off));
//	eeprom_read_block(&first_byte,offset,1);
	return first_byte != 0xff;
}


void initThermistorTables() {
	has_table[0] = isTableSet(eeprom_offsets::THERM_TABLE + therm_eeprom_offsets::THERM_DATA_OFFSET);
	//has_table[1] = isTableSet(eeprom::THERM_TABLE_1 + eeprom::THERM_DATA_OFFSET);
}

#endif
