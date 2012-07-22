#include "Pin.hh"

Pin::Pin() : port_base(NullPort.port_base), is_null(true), pin_index(0), pin_mask(0), pin_mask_inverted((uint8_t)~0) {}

Pin::Pin(const LPCPort& port_in, uint8_t pin_index_in) : port_base(port_in.port_base), is_null(port_base == NULL_PORT), pin_index(pin_index_in), pin_mask((uint8_t)_BV(pin_index_in)), pin_mask_inverted((uint8_t)~_BV(pin_index_in)) {}

Pin::Pin(const Pin& other_pin) : port_base(other_pin.port_base), is_null(port_base == NULL_PORT), pin_index(other_pin.pin_index), pin_mask(other_pin.pin_mask), pin_mask_inverted(other_pin.pin_mask_inverted) {}

bool Pin::isNull() const { return is_null; }

void Pin::setDirection(bool out) const {
	// if (is_null)
	// 	return;
//	uint8_t oldSREG = SREG;
//               cli();
//	xprintf("Pin::setDirection - port_base:%d pin_index:%d (%s:%d)\n",port_base,pin_index,_F_,_L_);
//	xprintf("Pin::setDirection - port_base:%d pin_index:%x (%s:%d)\n",port_base,_BV(pin_index),_F_,_L_);
//	if (out) {
//		DDRx |= (uint8_t)pin_mask;
//		GPIO_SetDir(port_base, (1 << pin_mask), 1);
//		GPIO_SetDir(port_base, _BV(pin_index), 1);
//	} else {
//		DDRx &= (uint8_t)pin_mask_inverted;
//		GPIO_SetDir(port_base, (1 << pin_mask_inverted), 0);
//		GPIO_SetDir(port_base, _BV(pin_index), 0);
//	}
	GPIO_SetDir(port_base, _BV(pin_index), out);
//	SREG = oldSREG;
}

/*
bool Pin::getValue() const {
	if (is_null)
		return false; // null pin is always low ... ?
	return (uint8_t)((uint8_t)PINx & (uint8_t)pin_mask) != 0;
}

void Pin::setValue(bool on) const {
	// if (is_null)
	// 	return;
	uint8_t oldSREG = SREG;
               cli();
	if (on) {
		PORTx |= pin_mask;
	} else {
		PORTx &= pin_mask_inverted;
	}
	SREG = oldSREG;
}
*/
