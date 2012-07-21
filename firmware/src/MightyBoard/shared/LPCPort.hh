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

#ifndef SHARED_AVR_PORT_HH_
#define SHARED_AVR_PORT_HH_

#define __STDC_LIMIT_MACROS
//#include <stdint.h>
//#include <util/atomic.h>

extern "C" {
	#include "lpc_types.h"
	#include "lpc17xx_gpio.h"
}


#define NULL_PORT 0xffffffff

class LPCPort {
private:
	const uint8_t port_base;
	friend class Pin;
public:
	LPCPort();
	LPCPort(uint8_t port_base_in);
	bool isNull() const;
	void setPinDirectionOut(uint8_t pin_mask) const;
	void setPinDirectionIn(uint8_t pin_mask_inverted) const;
	bool getPin(uint8_t pin_mask) const;
	void setPinOn(uint8_t pin_mask) const;
	void setPinOff(uint8_t pin_mask_inverted) const;
};


const LPCPort Port0;
const LPCPort Port1;
const LPCPort Port2;
const LPCPort Port3;
const LPCPort Port4;

const LPCPort NullPort;

#endif // SHARED_AVR_PORT_HH_

