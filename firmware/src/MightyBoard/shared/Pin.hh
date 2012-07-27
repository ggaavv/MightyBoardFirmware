#ifndef PIN_HH
#define PIN_HH

#include "LPCPort.hh"
#include "Pin.hh"
extern "C" {
	#include "lpc17xx_gpio.h"
	#include "lpc17xx_pinsel.h"
	#include "comm.h"
}

/// \ingroup HardwareLibraries
class Pin {
private:
	uint8_t port_base;
	bool is_null;
	uint8_t pin_index;
	uint8_t pin_mask;
	uint8_t pin_mask_inverted;

public:
	Pin();
	Pin(const LPCPort& port_in, uint8_t pin_index_in);
	Pin(const Pin& other_pin);
	bool isNull() const;
	void setDirection(bool out) const;
	bool /*Pin::*/getValue() const {
		if (is_null)
			return false; // null pin is always low ... ?
		return ((uint8_t)(FIO_ReadValue(port_base) & _BV(pin_index))?1:0);
//		return (uint8_t)((uint8_t)PINx & (uint8_t)pin_mask) != 0;
	};

	void /*Pin::*/setValue(bool on) const {
		if (is_null)
		 	return;
		// uint8_t oldSREG = SREG;

//		xprintf("on:%d port_base:%d pi_index:%d" " (%s:%d)\n",on,port_base,pin_index,_F_,_L_);
		// cli();
		if (on) {
//			PORTx |= pin_mask;
//			xprintf("%x (%s:%d)\n",&GPIO_SetValue,_F_,_L_);
			GPIO_SetValue(port_base, _BV(pin_index));
		} else {
//			PORTx &= pin_mask_inverted;
//			xprintf("%x (%s:%d)\n",&GPIO_ClearValue,_F_,_L_);
			GPIO_ClearValue(port_base, _BV(pin_index));
		}
		// SREG = oldSREG;
	};

	void /*Pin::*/setValueOn() const {
		if (is_null)
		 	return;
		// uint8_t oldSREG = SREG;
		// cli();
//		PORTx |= pin_mask;
		GPIO_SetValue(port_base, _BV(pin_index));
		// SREG = oldSREG;
	};

	void /*Pin::*/setValueOff() const {
		// if (is_null)
		// 	return;
		// uint8_t oldSREG = SREG;
		// cli();
//		PORTx &= pin_mask_inverted;
		GPIO_ClearValue(port_base, _BV(pin_index));
		// SREG = oldSREG;
	};
	// currently not used:
	//const uint8_t getPinIndex() const { return pin_index; }
};

static const Pin NullPin(NullPort, 0);

#endif // PIN_HH
