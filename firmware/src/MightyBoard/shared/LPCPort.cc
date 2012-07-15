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


#include "LPCPort.hh"

extern const LPCPort Port0(0);
extern const LPCPort Port1(1);
extern const LPCPort Port2(2);
extern const LPCPort Port3(3);
extern const LPCPort Port4(4);

extern const LPCPort NullPort = LPCPort();

LPCPort::LPCPort() : port_base(NULL_PORT) {};

LPCPort::LPCPort(port_base_t port_base_in) : port_base(port_base_in) {
};

bool LPCPort::isNull() const {
	return port_base == NULL_PORT;
};

void LPCPort::setPinDirectionOut(uint8_t pin_mask) const {
//	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
//		DDRx |= (uint8_t)pin_mask;
//	}
	GPIO_SetDir(port_base, (1 << pin_mask), 1)
};

void LPCPort::setPinDirectionIn(uint8_t pin_mask_inverted) const {
//	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
//		DDRx &= (uint8_t)pin_mask_inverted;
//	}
	GPIO_SetDir(port_base, (1 << pin_mask_inverted), 0)
};

bool LPCPort::getPin(uint8_t pin_mask) const {
//	return (uint8_t)((uint8_t)PINx & (uint8_t)pin_mask) != 0;
	return ((FIO_ReadValue(port_base) & (1 << pin_index))?1:0);
};

void LPCPort::setPinOn(uint8_t pin_mask) const {
//	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
//		PORTx |= (uint8_t)pin_mask;
//	}
	GPIO_SetValue(port_base, (1 << pin_mask));
};

void LPCPort::setPinOff(uint8_t pin_mask_inverted) const {
//	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
//		PORTx &= (uint8_t)pin_mask_inverted;
//	}
	GPIO_ClearValue(port_base, (1 << pin_mask_inverted));
};
