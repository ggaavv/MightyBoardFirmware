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

#include "AnalogPin.hh"
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/atomic.h>

#include "Configuration.hh"
extern "C" {
	#include "lpc_types.h"
	#include "lpc17xx_adc.h"
	#include "lpc17xx_pinsel.h"
	#include "lpc17xx_gpio.h"
	#include "comm.h"
}


volatile int16_t* adc_destination; //< Address to write the sampled data to

volatile bool* adc_finished; //< Flag to set once the data is sampled

volatile uint16_t adc_int_destination[3];
volatile bool adc_int_finished[3];
volatile bool adc_int_started[3];

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__)

    // We are using the AVcc as our reference.  There's a 100nF cap
    // to ground on the AREF pin.
    const uint8_t ANALOG_REF = 0x01;

    void initAnalogPin(uint8_t pin) {
            // Only analog pins 0-5 need to be initialized, 6 and 7 are dedicated purpose.
            if (pin < 6) {
                    DDRC &= ~(_BV(pin));
                    PORTC &= ~(_BV(pin));
            }

            // enable a2d conversions, interrupt on completion
            ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) |
                            _BV(ADEN) | _BV(ADIE);
    }

    bool startAnalogRead(uint8_t pin,
                         volatile int16_t* destination,
                         volatile bool* finished) {
            // ADSC is cleared when the conversion finishes.
            // We should not start a new read while an existing one is in progress.
            if ((ADCSRA & _BV(ADSC)) != 0) {
                    return false;
            }
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    adc_destination = destination;
                    adc_finished = finished;
                    *adc_finished = false;

                    // set the analog reference (high two bits of ADMUX) and select the
                    // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
                    // to 0 (the default).
                    ADMUX = (ANALOG_REF << 6) | (pin & 0x0f);

                    // start the conversion.
                    ADCSRA |= _BV(ADSC);
            }
            // An interrupt will signal conversion completion.
            return true;
    }

    ISR(ADC_vect)
    {
            uint8_t low_byte, high_byte;
            // we have to read ADCL first; doing so locks both ADCL
            // and ADCH until ADCH is read.  reading ADCL second would
            // cause the results of each conversion to be discarded,
            // as ADCL and ADCH would be locked when it completed.
            low_byte = ADCL;
            high_byte = ADCH;

            // combine the two bytes
            *adc_destination = (high_byte << 8) | low_byte;
            *adc_finished = true;
    }

#else
    // We are using the AVcc as our reference.  There's a 100nF cap
    // to ground on the AREF pin.
//    const uint8_t ANALOG_REF = 0x01;

void initAnalogPin(uint8_t pin) {
//	xprintf("initAnalogPin pin:%d" " (%s:%d)\n",pin,_F_,_L_);
            // Analog pins are on ports F and K
//            if (pin < 8) {
//                    DDRF &= ~(_BV(pin));
//                    PORTF &= ~(_BV(pin));
                    // clear ADC Channel bit selecting upper 8 ADCs
//					ADCSRB &= ~0b01000;
//            }
//            else{
//				pin -= 8;
//				DDRK &= ~(_BV(pin));
//				PORTK &= ~(_BV(pin));
				// set ADC Channel bit selecting upper 8 ADCs
//				ADCSRB |= 0b01000;
//			}
			
			// select ADC Channel and connect AREF to AVCC
//			ADMUX = 0b01000000 + pin;
            // enable a2d conversions, interrupt on completion
//            ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) |
//                            _BV(ADEN) | _BV(ADIE);
	  PINSEL_CFG_Type PinCfg;
	  PinCfg.Funcnum = PINSEL_FUNC_1; /* ADC function */
	  PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	  PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	  PinCfg.Portnum = 0;
	  PinCfg.Pinnum = pin;
	  PINSEL_ConfigPin(&PinCfg);
	  GPIO_SetDir(0, _BV(pin), 0);

	  ADC_Init(LPC_ADC, 200000); /* ADC conversion rate = 200Khz */
//	  ADC_IntConfig(LPC_ADC,pin-23,ENABLE);
	  // ADC does not like being enabled/disable multiple times
	  ADC_ChannelCmd(LPC_ADC,1,ENABLE);
	  ADC_ChannelCmd(LPC_ADC,2,ENABLE);
	  ADC_ChannelCmd(LPC_ADC,3,ENABLE);
	  ADC_BurstCmd(LPC_ADC,ENABLE);
//	  ADC_StartCmd(LPC_ADC,ADC_START_NOW);
//	  NVIC_SetPriority(ADC_IRQn, 17);
//	  NVIC_EnableIRQ(ADC_IRQn);
    }

    bool startAnalogRead(uint8_t pin,
                         volatile int16_t* destination,
                         volatile bool* finished) {
//    	xprintf("pin:%d" " (%s:%d)\n",pin-23,_F_,_L_);
            // ADSC is cleared when the conversion finishes.
            // We should not start a new read while an existing one is in progress.
//    		  if (ADC_ChannelGetData(LPC_ADC,pin-23) == false)
//    			  return false;
//            if ((ADCSRA & _BV(ADSC)) != 0) {
//                    return false;
//            }
//            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    adc_destination = destination;
                    adc_finished = finished;
                    *adc_finished = false;

//                    if (pin < 8) {
						// clear ADC Channel bit selecting upper 8 ADCs
//						ADCSRB &= ~0b01000;
//					}
//					else{
//						pin -= 8;
						// set ADC Channel bit selecting upper 8 ADCs
//						ADCSRB |= 0b01000;
//					}
			
					// select ADC Channel and connect AREF to AVCC
//					ADMUX = 0b01000000 + pin;

                    // start the conversion.
//                    ADCSRA |= _BV(ADSC);
//            }
              // Start conversion
              if (ADC_ChannelGetStatus(LPC_ADC,pin-23,1) == true){
//            	  xprintf("adc_int_finished pin:%d" " (%s:%d)\n",pin-23,_F_,_L_);
            	  *adc_destination = ADC_ChannelGetData(LPC_ADC,pin-23);
            	  *adc_finished = true;
            	  return true;
              }
            // An interrupt will signal conversion completion.
             return false;
    }

/*    ISR(ADC_vect)
    {
            uint8_t low_byte, high_byte;
            // we have to read ADCL first; doing so locks both ADCL
            // and ADCH until ADCH is read.  reading ADCL second would
            // cause the results of each conversion to be discarded,
            // as ADCL and ADCH would be locked when it completed.
            low_byte = ADCL;
            high_byte = ADCH;

            // combine the two bytes
            *adc_destination = (high_byte << 8) | low_byte;
            *adc_finished = true;
    }*/

#endif

