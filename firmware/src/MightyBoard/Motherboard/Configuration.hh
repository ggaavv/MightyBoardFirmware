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

#ifndef BOARDS_LPC1768_CONFIGURATION_HH_
#define BOARDS_LPC1768_CONFIGURATION_HH_

// This file details the pin assignments and features of the Makerbot Extended Motherboard 4.0

#include "LPCPort.hh"

// Interval for the stepper update in microseconds.  This interval is the minimum
// possible time between steps; in practical terms, your time between steps should
// be at least eight times this large.  Reducing the interval can cause resource
// starvation; leave this at 64uS or greater unless you know what you're doing.
#define INTERVAL_IN_MICROSECONDS 128
#define HOMING_INTERVAL_IN_MICROSECONDS 128

// --- Power Supply Unit configuration ---
// Define as 1 if a PSU is present; 0 if not.
#define HAS_PSU         0

// --- Secure Digital Card configuration ---
// NOTE: If SD support is enabled, it is implicitly assumed that the
// following pins are connected:
//  AVR    |   SD header
//---------|--------------
//  MISO   |   DATA_OUT
//  MOSI   |   DATA_IN
//  SCK    |   CLK


//		LPCMini LED's
//#define DEBUG_LED1 Pin(Port1,18)
//#define DEBUG_LED2 Pin(Port1,20)
//#define DEBUG_LED3 Pin(Port1,21)
//#define DEBUG_LED4 Pin(Port1,23)

// Define as 1 if and SD card slot is present; 0 if not.
#define HAS_SD          1
// The pin that connects to the write protect line on the SD header.
#define SD_WRITE_PIN    Pin(Port1,0)
// The pin that connects to the card detect line on the SD header.
#define SD_DETECT_PIN   Pin(Port2,1)
// The pin that connects to the chip select line on the SD header.
#define SD_SELECT_PIN   Pin(Port2,0)

//#define MOSI_PIN   Pin(Port1,0)
//#define SCK_PIN   Pin(Port1,0)
//#define SS_PIN   Pin(Port1,0)
//#define MISO_PIN   Pin(Port1,0)

//USB
//#define USB_DISC_PIN Pin(Port1,7)

// --- Slave UART configuration ---
// The slave UART is presumed to be an RS485 connection through a sn75176 chip.
// Define as 1 if the slave UART is present; 0 if not.
#define HAS_SLAVE_UART 1
// The pin that connects to the driver enable line on the RS485 chip.
//#define TX_ENABLE_PIN   Pin(NULL_PORT,NULL_PORT)
#define TX_ENABLE_PIN   Pin(Port1,21)
// The pin that connects to the active-low recieve enable line on the RS485 chip.
#define RX_ENABLE_PIN   Pin(Port2,8)

// --- Host UART configuration ---
// The host UART is presumed to always be present on the RX/TX lines.

// --- Piezo Buzzer configuration ---
// Define as 1 if the piezo buzzer is present, 0 if not.
#define HAS_BUZZER 1
// The pin that drives the buzzer
#define BUZZER_PIN Pin(Port2,5)  //OC0B

// --- Axis configuration ---
// Define the number of stepper axes supported by the board.  The axes are
// denoted by X, Y, Z, A and B.
#define STEPPER_COUNT   4
#define MAX_STEPPERS    4

// --- Stepper and endstop configuration ---
// Pins should be defined for each axis present on the board.  They are denoted
// X, Y, Z, A and B respectively.

// This indicates the default interpretation of the endstop values.
// If your endstops are based on the H21LOB, they are inverted;
// if they are based on the H21LOI, they are not.
#define DEFAULT_INVERTED_ENDSTOPS 1

// The X stepper step pin (active on rising edge)
#define X_STEP_PIN      Pin(Port2,13)
// The X direction pin (forward on logic high)
#define X_DIR_PIN       Pin(Port2,12)
// The X stepper enable pin (active low)
#define X_ENABLE_PIN    Pin(Port2,11)
// X stepper potentiometer pin
#define X_POT_PIN		Pin(NULL_PORT,NULL_PORT)
// The X minimum endstop pin (active high)
#define X_MIN_PIN       Pin(Port0,22)
// The X maximum endstop pin (active high)
#define X_MAX_PIN       Pin(Port0,11)

// The Y stepper step pin (active on rising edge)
#define Y_STEP_PIN      Pin(Port1,28)
// The Y direction pin (forward on logic high)
#define Y_DIR_PIN       Pin(Port1,29)
// The Y stepper enable pin (active low)
#define Y_ENABLE_PIN    Pin(Port0,10)
// Y stepper potentiometer pin
#define Y_POT_PIN		Pin(NULL_PORT,NULL_PORT)
// The Y minimum endstop pin (active high)
#define Y_MIN_PIN       Pin(Port1,27)
// The Y maximum endstop pin (active high)
#define Y_MAX_PIN       Pin(Port1,23)

// The Z stepper step pin (active on rising edge)
#define Z_STEP_PIN      Pin(Port1,24)
// The Z direction pin (forward on logic high)
#define Z_DIR_PIN       Pin(Port1,25)
// The Z stepper enable pin (active low)
#define Z_ENABLE_PIN    Pin(Port1,26)
// Z stepper potentiometer pin
#define Z_POT_PIN		Pin(NULL_PORT,NULL_PORT)
// The Z minimum endstop pin (active high)
#define Z_MIN_PIN       Pin(Port1,22)
// The Z maximum endstop pin (active high)
#define Z_MAX_PIN       Pin(Port3,25)

// The A stepper step pin (active on rising edge)
#define A_STEP_PIN      Pin(Port1,19)
// The A direction pin (forward on logic high)
//#define A_DIR_PIN       Pin(NULL_PORT,NULL_PORT)
#define A_DIR_PIN       Pin(Port1,20)
// The A stepper enable pin (active low)
//#define A_ENABLE_PIN    Pin(NULL_PORT,NULL_PORT)
#define A_ENABLE_PIN    Pin(Port1,21)
// A stepper potentiometer pin
#define A_POT_PIN		Pin(NULL_PORT,NULL_PORT)

// The B stepper step pin (active on rising edge)
#define B_STEP_PIN      Pin(Port0,27)
// The B direction pin (forward on logic high)
#define B_DIR_PIN       Pin(Port0,28)
// The B stepper enable pin (active low)
#define B_ENABLE_PIN    Pin(Port3,26)
// B stepper potentiometer pin
#define B_POT_PIN       Pin(NULL_PORT,NULL_PORT)

// i2c pots SCL pin
#define POTS_SCL        Pin(NULL_PORT,NULL_PORT)
// default value for pots (0-127 valid)
#define POTS_DEFAULT_VAL 50

// --- Debugging configuration ---
// The pin which controls the debug LED (active high)
#define DEBUG_PIN       Pin(Port1,27)
//#define DEBUG_PIN	Pin(NULL_PORT,NULL_PORT)
// Additional Debug Pins
#define DEBUG_PIN1	Pin(NULL_PORT,NULL_PORT)
#define DEBUG_PIN2	Pin(NULL_PORT,NULL_PORT)
#define DEBUG_PIN3	Pin(NULL_PORT,NULL_PORT)
#define DEBUG_PIN4  Pin(NULL_PORT,NULL_PORT)
#define DEBUG_PIN5  Pin(NULL_PORT,NULL_PORT)
#define DEBUG_PIN6  Pin(NULL_PORT,NULL_PORT)
#define DEBUG_PIN7  Pin(NULL_PORT,NULL_PORT)
#define DEBUG_PIN8  Pin(NULL_PORT,NULL_PORT)

// By default, debugging packets should be honored; this is made
// configurable if we're short on cycles or EEPROM.
// Define as 1 if debugging packets are honored; 0 if not.
#define HONOR_DEBUG_PACKETS 1

#define HAS_INTERFACE_BOARD     0

#define LCD_RS_PIN			Pin(Port0,2)
#define LCD_ENABLE_PIN		Pin(Port0,3)
#define LCD_D0_PIN			Pin(Port1,4)
#define LCD_D1_PIN			Pin(Port1,10)
#define LCD_D2_PIN			Pin(Port1,9)
#define LCD_D3_PIN			Pin(Port1,15)

// LCD interface pins
#define LCD_STROBE		Pin(NULL_PORT,NULL_PORT)
#define LCD_CLK			Pin(NULL_PORT,NULL_PORT)
#define LCD_DATA		Pin(NULL_PORT,NULL_PORT)

/// This is the pin mapping for the interface board. Because of the relatively
/// high cost of using the pins in a direct manner, we will instead read the
/// buttons directly by scanning their ports. If any of these definitions are
/// modified, the #scanButtons() function _must_ be updated to reflect this.
#define INTERFACE_UP		Pin(Port1,16)
#define INTERFACE_DOWN		Pin(Port0,5)
#define INTERFACE_RIGHT		Pin(Port1,14)
#define INTERFACE_LEFT		Pin(Port1,17)
#define INTERFACE_CENTER	Pin(Port1,1)

#define INTERFACE_GLED		Pin(NULL_PORT,NULL_PORT)
#define INTERFACE_RLED		Pin(NULL_PORT,NULL_PORT)

#define INTERFACE_DETECT	Pin(NULL_PORT,NULL_PORT)

/// Character LCD screen geometry
#define LCD_SCREEN_WIDTH        20
#define LCD_SCREEN_HEIGHT       4

///// **** HBP and Extruder  ***************/////

/// True if there are any thermistors on the board
#define HAS_THERMISTOR_TABLES

// Extruder thermistor analog pin
#define THERMISTOR_PIN			26 // ADC3 Extruder0
#define THERMISTOR_PIN_E2		25 // ADC2 Extruder0
//#define THERMISTOR_PIN			Pin(Port0,26)

#define HAS_THERMOCOUPLE        0

#define THERMOCOUPLE_CS1        Pin(NULL_PORT,NULL_PORT)
#define THERMOCOUPLE_CS2        Pin(NULL_PORT,NULL_PORT)
#define THERMOCOUPLE_SCK        Pin(NULL_PORT,NULL_PORT)
#define THERMOCOUPLE_SO         Pin(NULL_PORT,NULL_PORT)

#define DEFAULT_THERMOCOUPLE_VAL	1024

// Platform thermistor analog pin
#define PLATFORM_PIN            24

/// POWER Pins for extruders, fans and heated build platform
#define EX1_PWR	                Pin(Port2,3) // OC4A
#define EX2_PWR	                Pin(Port2,4) // OC1A
#define EX1_FAN                 Pin(NULL_PORT,NULL_PORT) // EX1_FAN
#define HBP_HEAT                Pin(NULL_PORT,NULL_PORT) // OC5B
#define EX2_FAN                 Pin(NULL_PORT,NULL_PORT)
#define EXTRA_FET               Pin(NULL_PORT,NULL_PORT)

// sample intervals for heaters
#define SAMPLE_INTERVAL_MICROS_THERMISTOR (50L * 1000L)
#define SAMPLE_INTERVAL_MICROS_THERMOCOUPLE (500L * 1000L)

// Safety Cutoff circuit
#ifndef CUTOFF_PRESENT
  #define CUTOFF_PRESENT			0
#endif // CUTOFF_PRESENT

// safety cutoff circuit 
#define CUTOFF_RESET			Pin(NULL_PORT,NULL_PORT)
#define CUTOFF_TEST				Pin(NULL_PORT,NULL_PORT)
#define CUTOFF_SR_CHECK			Pin(NULL_PORT,NULL_PORT)

#define EX2_PWR_CHECK			Pin(NULL_PORT,NULL_PORT)
#define EX1_PWR_CHECK			Pin(NULL_PORT,NULL_PORT)

// bot shuts down printers after a defined timeout 
#define USER_INPUT_TIMEOUT		1800000000 // 30 minutes

#define XSTEPS_PER_MM          64.15f
#define YSTEPS_PER_MM          45.93f
#define ZSTEPS_PER_MM          2560
#define ASTEPS_PER_MM          330.49f
#define BSTEPS_PER_MM          330.49f


#endif // BOARDS_LPC1768_CONFIGURATION_HH_
