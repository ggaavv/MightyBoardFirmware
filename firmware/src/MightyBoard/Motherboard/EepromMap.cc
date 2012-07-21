/*
 * Copyright 2010 by Adam Mayer <adam@makerbot.com>
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

#include "EepromMap.hh"
#include "Eeprom.hh"
//#include <avr/eeprom.h>
//#include <avr/delay.h>

//for thermistor generation
#include "ThermistorTable.hh"

// for sound definition
#include "Piezo.hh"

// for LED definition
#include "RGB_LED.hh"

// for cooling fan definition
#include "CoolingFan.hh"
#include "IAP.hh"

extern "C" {
	#include "lpc_types.h"
}

namespace eeprom {

#define DEFAULT_P_VALUE  (7.0f)
#define DEFAULT_I_VALUE  (0.325f)
#define DEFAULT_D_VALUE  (36.0f)


#define THERM_R0_DEFAULT_VALUE (100000)
#define THERM_T0_DEFAULT_VALUE (25)
#define THERM_BETA_DEFAULT_VALUE (4067)


void read_all_from_flash (void){
	__disable_irq ();
	uint32_t i;
	for (i = 0x00000000; i < eeprom_info::EEPROM_SIZE; i++,i++,i++,i++) {
		eeprom_address(i) = eeprom_address(EEPROM_FLASH_AREA_START+i - EEPROM_START_ADDRESS);
	}
	__enable_irq ();
};

void save_to_flash (void) {
	__disable_irq ();
	IAP in_ap_prog;
	int error_code_ret = in_ap_prog.erase(USER_FLASH_AREA_START, USER_FLASH_AREA_START);
	error_code_ret = in_ap_prog.write((char)0x10007000, (char)USER_FLASH_AREA_START, (int)USER_FLASH_AREA_SIZE );
	// read all variables back into Ram
	__enable_irq ();
};
/*
uint8_t microstep_pinout(uint8_t port_no) {
	uint8_t microsteping_port;
	for (uint8_t ii = 0; ii < 5; ii++){   //Counts through STEPPER_COUNT
		for (uint8_t iii = 0; ii < 3; iii++){
			uint8_t port_bit = 0x1 << microstep_port_array [ii][iii][1];
			if (microstep_port_array [ii][iii][0] == port_no) {
				microsteping_port += (microstep_port_array [ii][iii][2] << port_bit);
			}
		}
	}
	return microsteping_port;
}
*/
/**
 *
 * @param eeprom_base start of eeprom map of cooling settings
 */
void setDefaultCoolingFan(uint16_t eeprom_base, uint8_t save_now){

	uint8_t fan_settings[] = {1, DEFAULT_COOLING_FAN_SETPOINT_C};
	eeprom_address(eeprom_base + cooler_eeprom_offsets::ENABLE_OFFSET) = fan_settings[0];
	eeprom_address(eeprom_base + cooler_eeprom_offsets::ENABLE_OFFSET+1) = fan_settings[1];
//    eeprom_write_block( fan_settings, (uint8_t*)(eeprom_base + cooler_eeprom_offsets::ENABLE_OFFSET),2);
	if (save_now)
		save_to_flash();
}


/**
 * Start of PID block of EEPROM. Can be extruder or HPB
 * @param eeprom_base
 */
void setDefaultPID(uint16_t eeprom_base, uint8_t save_now)
{
	eeprom_address(eeprom_base + pid_eeprom_offsets::P_TERM_OFFSET) = DEFAULT_P_VALUE;
	eeprom_address(eeprom_base + pid_eeprom_offsets::I_TERM_OFFSET) = DEFAULT_I_VALUE;
	eeprom_address(eeprom_base + pid_eeprom_offsets::D_TERM_OFFSET) = DEFAULT_D_VALUE;
//	setEepromFixed16(( eeprom_base + pid_eeprom_offsets::P_TERM_OFFSET ), DEFAULT_P_VALUE);
//	setEepromFixed16(( eeprom_base + pid_eeprom_offsets::I_TERM_OFFSET ), DEFAULT_I_VALUE);
//	setEepromFixed16(( eeprom_base + pid_eeprom_offsets::D_TERM_OFFSET ), DEFAULT_D_VALUE);
	if (save_now)
		save_to_flash();
}


/**
 *
 * @param index
 * @param eeprom_base start of this extruder's data in the eeprom
 */
void setDefaultsExtruder(int index,uint16_t eeprom_base, uint8_t save_now)
{
	uint8_t featuresT0 = eeprom_info::HEATER_0_PRESENT | eeprom_info::HEATER_0_THERMISTOR | eeprom_info::HEATER_0_THERMOCOUPLE;
	uint8_t featuresT1 = eeprom_info::HEATER_1_PRESENT | eeprom_info::HEATER_1_THERMISTOR | eeprom_info::HEATER_1_THERMOCOUPLE;
	if( index == 0 ){
		int slaveId = '12';
		eeprom_address(eeprom_base + toolhead_eeprom_offsets::FEATURES) = featuresT0;
		eeprom_address(eeprom_base + toolhead_eeprom_offsets::SLAVE_ID) = slaveId;
//	    eeprom_write_byte( (uint8_t*)(eeprom_base + toolhead_eeprom_offsets::FEATURES),featuresT0);
//		eeprom_write_byte( (uint8_t*)eeprom_base +toolhead_eeprom_offsets::SLAVE_ID,slaveId);
	}
	else{
		int slaveId = '32';
		eeprom_address(eeprom_base + toolhead_eeprom_offsets::FEATURES) = featuresT1;
		eeprom_address(eeprom_base + toolhead_eeprom_offsets::SLAVE_ID) = slaveId;
//		eeprom_write_byte( (uint8_t*)(eeprom_base + toolhead_eeprom_offsets::FEATURES),featuresT1);
//		eeprom_write_byte( (uint8_t*)eeprom_base +toolhead_eeprom_offsets::SLAVE_ID,slaveId);
	}
	setDefaultPID((eeprom_base + toolhead_eeprom_offsets::EXTRUDER_PID_BASE),save_now);
    setDefaultPID((eeprom_base + toolhead_eeprom_offsets::HBP_PID_BASE),save_now);
    setDefaultCoolingFan(eeprom_base + toolhead_eeprom_offsets::COOLING_FAN_SETTINGS,save_now);

	eeprom_address(eeprom_base + toolhead_eeprom_offsets::BACKOFF_FORWARD_TIME) = 500;
	eeprom_address(eeprom_base + toolhead_eeprom_offsets::BACKOFF_STOP_TIME) = 5;
	eeprom_address(eeprom_base + toolhead_eeprom_offsets::BACKOFF_REVERSE_TIME) = 500;
	eeprom_address(eeprom_base + toolhead_eeprom_offsets::BACKOFF_TRIGGER_TIME) = 300;
//    eeprom_write_word((uint16_t*)(eeprom_base + toolhead_eeprom_offsets::BACKOFF_FORWARD_TIME),500);
//    eeprom_write_word((uint16_t*)(eeprom_base + toolhead_eeprom_offsets::BACKOFF_STOP_TIME),5);
//    eeprom_write_word((uint16_t*)(eeprom_base + toolhead_eeprom_offsets::BACKOFF_REVERSE_TIME),500);
//    eeprom_write_word((uint16_t*)(eeprom_base + toolhead_eeprom_offsets::BACKOFF_TRIGGER_TIME),300);

	if (save_now)
		save_to_flash();
}


/**
 * Set thermal table offsets
 * @param eeprom_base
 */
void SetDefaultsThermal(uint16_t eeprom_base, uint8_t save_now)
{
	eeprom_address(eeprom_base + therm_eeprom_offsets::THERM_R0_OFFSET) = THERM_R0_DEFAULT_VALUE;
	eeprom_address(eeprom_base + therm_eeprom_offsets::THERM_T0_OFFSET) = THERM_T0_DEFAULT_VALUE;
	eeprom_address(eeprom_base + therm_eeprom_offsets::THERM_BETA_OFFSET) = THERM_BETA_DEFAULT_VALUE;
//	eeprom_write_dword( (uint32_t*)(eeprom_base + therm_eeprom_offsets::THERM_R0_OFFSET), THERM_R0_DEFAULT_VALUE);
//	eeprom_write_dword( (uint32_t*)(eeprom_base + therm_eeprom_offsets::THERM_T0_OFFSET), THERM_T0_DEFAULT_VALUE);
//	eeprom_write_dword( (uint32_t*)(eeprom_base + therm_eeprom_offsets::THERM_BETA_OFFSET), THERM_BETA_DEFAULT_VALUE);
	/// write the default thermal table.
	for(uint8_t i=0;i<sizeof(uint16_t)*NUMTEMPS*2;i++,i++,i++,i++){
		eeprom_address(eeprom_base + therm_eeprom_offsets::THERM_DATA_OFFSET+i) = default_therm_table[i].adc;
		eeprom_address(eeprom_base + therm_eeprom_offsets::THERM_DATA_OFFSET+i+2) = default_therm_table[i].value;
	}
	//	eeprom_write_block( (const uint8_t*)default_therm_table,
//			(uint8_t*)(eeprom_base + therm_eeprom_offsets::THERM_DATA_OFFSET), sizeof(uint16_t)*2*NUMTEMPS);
	if (save_now)
		save_to_flash();
}

typedef struct Color {
	int8_t red;
	int8_t green;
	int8_t blue;
} Color;
        


/**
 *
 * @param eeprom_base start of Led effects table
 */
void setDefaultLedEffects(uint16_t eeprom_base, uint8_t save_now)
{
	Color colors;

	// default color is white
	eeprom_address(eeprom_base + blink_eeprom_offsets::BASIC_COLOR_OFFSET) = LED_DEFAULT_WHITE;
	eeprom_address(eeprom_base + blink_eeprom_offsets::LED_HEAT_OFFSET) = 1;
//	eeprom_write_byte((uint8_t*)(eeprom_base + blink_eeprom_offsets::BASIC_COLOR_OFFSET), LED_DEFAULT_WHITE);
//	eeprom_write_byte((uint8_t*)(eeprom_base + blink_eeprom_offsets::LED_HEAT_OFFSET), 1);
    
	colors.red=0xFF; colors.green =colors.blue =0x00;
	for(uint8_t i=0;i<sizeof(colors)*3;i++,i++,i++){
		eeprom_address(eeprom_base + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET+i) = colors.red;
		eeprom_address(eeprom_base + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET+i+1) = colors.green;
		eeprom_address(eeprom_base + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET+i+2) = colors.blue;
	}
	//	eeprom_write_block((void*)&colors,(uint8_t*)(eeprom_base + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET),sizeof(colors));
	if (save_now)
		save_to_flash();
}
    /**
     *
     * @param red value
     * @param green value
     * @param blue value
     */

void setCustomColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t save_now){
	
	Color colors;
	
	eeprom_address(eeprom_offsets::LED_STRIP_SETTINGS + blink_eeprom_offsets::BASIC_COLOR_OFFSET) = LED_DEFAULT_CUSTOM;
//	eeprom_write_byte((uint8_t*)(eeprom_offsets::LED_STRIP_SETTINGS + blink_eeprom_offsets::BASIC_COLOR_OFFSET), LED_DEFAULT_CUSTOM);
	
	colors.red=red; colors.green = green; colors.blue =blue;
	for(uint8_t i=0;i<sizeof(colors)*3;i++,i++,i++){
		eeprom_address(eeprom_offsets::LED_STRIP_SETTINGS + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET+i) = colors.red;
		eeprom_address(eeprom_offsets::LED_STRIP_SETTINGS + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET+i+1) = colors.green;
		eeprom_address(eeprom_offsets::LED_STRIP_SETTINGS + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET+i+2) = colors.blue;
	}
	//	eeprom_write_block((void*)&colors,(uint8_t*)(eeprom_offsets::LED_STRIP_SETTINGS + blink_eeprom_offsets::CUSTOM_COLOR_OFFSET),sizeof(colors));
	if (save_now)
		save_to_flash();
}

    /**
     *
     * @param sound desired
     * @param dest in eeprom
     */   
void eeprom_write_sound(Sound sound, uint16_t dest, uint8_t save_now)
{
	eeprom_address(dest) = sound.freq;
	eeprom_address(dest + 2) = sound.durationMs;
//	eeprom_write_word((uint16_t*)dest, 	sound.freq);
//	eeprom_write_word((uint16_t*)dest + 2, sound.durationMs);
	if (save_now)
		save_to_flash();
}

/**
 *
 * @param eeprom_base start of buzz effects table
 */
void setDefaultBuzzEffects(uint16_t eeprom_base, uint8_t save_now)
{
	Sound blare = {NOTE_B2, 500};
	eeprom_write_sound(blare,eeprom_base + buzz_eeprom_offsets::BASIC_BUZZ_OFFSET, save_now);
	if (save_now)
		save_to_flash();
}
    
/**
 *
 * @param eeprom_base start of preheat settings table
 */
void setDefaultsPreheat(uint16_t eeprom_base, uint8_t save_now)
{
	eeprom_address(eeprom_base + preheat_eeprom_offsets::PREHEAT_RIGHT_OFFSET) = 220;
	eeprom_address(eeprom_base + preheat_eeprom_offsets::PREHEAT_LEFT_OFFSET) = 220;
	eeprom_address(eeprom_base + preheat_eeprom_offsets::PREHEAT_PLATFORM_OFFSET) = 110;
	eeprom_address(eeprom_base + preheat_eeprom_offsets::PREHEAT_ON_OFF_OFFSET) = (1<<HEAT_MASK_RIGHT) + (1<<HEAT_MASK_PLATFORM);
//    eeprom_write_word((uint16_t*)(eeprom_base + preheat_eeprom_offsets::PREHEAT_RIGHT_OFFSET), 220);
//    eeprom_write_word((uint16_t*)(eeprom_base + preheat_eeprom_offsets::PREHEAT_LEFT_OFFSET), 220);
//    eeprom_write_word((uint16_t*)(eeprom_base + preheat_eeprom_offsets::PREHEAT_PLATFORM_OFFSET), 110);
//    eeprom_write_byte((uint8_t*)(eeprom_base + preheat_eeprom_offsets::PREHEAT_ON_OFF_OFFSET), (1<<HEAT_MASK_RIGHT) + (1<<HEAT_MASK_PLATFORM));
	if (save_now)
		save_to_flash();
}


/**
 *
 * break with the form here as eeprom_base is available in class and we
 * want to cleanly call this function externally
 */
void setDefaultsAcceleration(uint8_t save_now)
{
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::ACTIVE_OFFSET) = 0x00;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::ACCELERATION_RATE_OFFSET) = DEFAULT_ACCELERATION;
//    eeprom_write_byte((uint8_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::ACTIVE_OFFSET), 0x00);
//    eeprom_write_word((uint16_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::ACCELERATION_RATE_OFFSET), DEFAULT_ACCELERATION);
    
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 0) = DEFAULT_X_ACCELERATION;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 2) = DEFAULT_Y_ACCELERATION;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 4) = DEFAULT_Z_ACCELERATION;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 6) = DEFAULT_A_ACCELERATION;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 8) = DEFAULT_B_ACCELERATION;
//  eeprom_write_word((uint16_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 0), DEFAULT_X_ACCELERATION);
//	eeprom_write_word((uint16_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 2), DEFAULT_Y_ACCELERATION);
//	eeprom_write_word((uint16_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 4), DEFAULT_Z_ACCELERATION);
//	eeprom_write_word((uint16_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 6), DEFAULT_A_ACCELERATION);
//	eeprom_write_word((uint16_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 8), DEFAULT_B_ACCELERATION);

	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 0) = DEFAULT_MAX_XY_JERK;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 4) = DEFAULT_MAX_Z_JERK;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 6) = DEFAULT_MAX_A_JERK;
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_RATES_OFFSET + 8) = DEFAULT_MAX_B_JERK;
//	setEepromFixed16((eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_JERK_OFFSET + 0), DEFAULT_MAX_XY_JERK);
//	setEepromFixed16((eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_JERK_OFFSET + 4), DEFAULT_MAX_Z_JERK);
//	setEepromFixed16((eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_JERK_OFFSET + 6), DEFAULT_MAX_A_JERK);
//	setEepromFixed16((eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::AXIS_JERK_OFFSET + 8), DEFAULT_MAX_B_JERK);
	
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::MINIMUM_SPEED) = DEFAULT_MIN_SPEED;
//	eeprom_write_word((uint16_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::MINIMUM_SPEED), DEFAULT_MIN_SPEED);
	
	eeprom_address(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::DEFAULTS_FLAG) = _BV(ACCELERATION_INIT_BIT);
//	eeprom_write_byte((uint8_t*)(eeprom_offsets::ACCELERATION_SETTINGS + acceleration_eeprom_offsets::DEFAULTS_FLAG), _BV(ACCELERATION_INIT_BIT));
	if (save_now)
		save_to_flash();
}

/// Writes to EEPROM the default toolhead 'home' values to idicate toolhead offset
/// from idealized point-center of the toolhead
void setDefaultAxisHomePositions(uint8_t save_now)
{
	uint32_t homes[5] = {replicator_axis_offsets::DUAL_X_OFFSET_STEPS,replicator_axis_offsets::DUAL_Y_OFFSET_STEPS,0,0,0};
	if(isSingleTool()){
		homes[0] = replicator_axis_offsets::SINGLE_X_OFFSET_STEPS;
		homes[1] = replicator_axis_offsets::SINGLE_Y_OFFSET_STEPS;
	}
	if (save_now)
	for (uint8_t i=0;i<20;i++){
		eeprom_address(eeprom_offsets::AXIS_HOME_POSITIONS_STEPS+i) = homes[i];
	}
//	eeprom_write_block((uint8_t*)&(homes[0]),(uint8_t*)(eeprom_offsets::AXIS_HOME_POSITIONS_STEPS), 20 );
	if (save_now)
		save_to_flash();
}
    
/// Does a factory reset (resets all defaults except home/endstops, axis direction and tool count)
void factoryResetEEPROM(uint8_t save_now) {

	// Default: enstops inverted, Z axis inverted
	uint8_t endstop_invert = 0b10011111; // all endstops inverted

	uint8_t home_direction = 0b11011; // X,Y Max, Z min  (AB max - to never halt on edge in stepper interface)

	uint8_t vRefBase[] = {118,118,40,118,118};  //(AB maxed out)
	uint16_t vidPid[] = {0x23C1, 0xB404};		/// PID/VID for the MightyBoard!

	/// Write 'MainBoard' settings
	char machine_name_in[] = "The Replicator";
	for (uint8_t i=0;i<20;i++){
		eeprom_address(eeprom_offsets::MACHINE_NAME+i) = machine_name_in[i];
	}
//	eeprom_write_block("The Replicator",(uint8_t*)eeprom_offsets::MACHINE_NAME,20); // name is null
	for (uint8_t i=0;i<5;i++){
		eeprom_address(eeprom_offsets::DIGI_POT_SETTINGS+i) = vRefBase[i];
	}
//	eeprom_write_block(&(vRefBase[0]),(uint8_t*)(eeprom_offsets::DIGI_POT_SETTINGS), 5 );
	eeprom_address(eeprom_offsets::ENDSTOP_INVERSION) = endstop_invert;
//	eeprom_write_byte((uint8_t*)eeprom_offsets::ENDSTOP_INVERSION, endstop_invert);
	eeprom_address(eeprom_offsets::AXIS_HOME_DIRECTION) = home_direction;
//	eeprom_write_byte((uint8_t*)eeprom_offsets::AXIS_HOME_DIRECTION, home_direction);
    

	setDefaultAxisHomePositions(save_now);
    
    /// store the default axis lengths for the machine
    for (uint8_t i=0;i<20;i++){
    	eeprom_address(eeprom_offsets::AXIS_LENGTHS) = replicator_axis_lengths::axis_lengths[i];
    }
//    eeprom_write_block((uint8_t*)&(replicator_axis_lengths::axis_lengths[0]), (uint8_t*)(eeprom_offsets::AXIS_LENGTHS), 20);
    
    setDefaultsAcceleration(save_now);
	
    eeprom_address(eeprom_offsets::FILAMENT_HELP_SETTINGS) = 1;
//	eeprom_write_byte((uint8_t*)eeprom_offsets::FILAMENT_HELP_SETTINGS, 1);

    /// Thermal table settings
    SetDefaultsThermal(eeprom_offsets::THERM_TABLE, save_now);
    
    /// Preheat heater settings
    setDefaultsPreheat(eeprom_offsets::PREHEAT_SETTINGS, save_now);

    /// write MightyBoard VID/PID. Only after verification does production write
    /// a proper 'The Replicator' PID/VID to eeprom, and to the USB chip
    for (uint8_t i=0;i<2;i++){
    	eeprom_address(eeprom_offsets::VID_PID_INFO+i+i) = vidPid[i];
    }
//    eeprom_write_block(&(vidPid[0]),(uint8_t*)eeprom_offsets::VID_PID_INFO,4);

    /// Write 'extruder 0' settings
    setDefaultsExtruder(0,eeprom_offsets::T0_DATA_BASE, save_now);

    /// Write 'extruder 1' stttings
    setDefaultsExtruder(1,eeprom_offsets::T1_DATA_BASE, save_now);

    /// write blink and buzz defaults
    setDefaultLedEffects(eeprom_offsets::LED_STRIP_SETTINGS, save_now);
    setDefaultBuzzEffects(eeprom_offsets::BUZZ_SETTINGS, save_now);
    
    // startup script flag is cleared
    eeprom_address(eeprom_offsets::FIRST_BOOT_FLAG) = save_now;
//    eeprom_write_byte((uint8_t*)eeprom_offsets::FIRST_BOOT_FLAG, 0);
    save_to_flash();
}

void setToolHeadCount(uint8_t count, uint8_t save_now){
	
	// update toolhead count
	if(count > 2)
		count = 1;
	eeprom_address(eeprom_offsets::TOOL_COUNT) = count;
//	eeprom_write_byte((uint8_t*)eeprom_offsets::TOOL_COUNT, count);
	
	// update XY axis offsets to match tool head settins
	setDefaultAxisHomePositions(save_now);
	
	if (save_now)
		save_to_flash();
}

    // check single / dual tool status
bool isSingleTool(){
	return (getEeprom8(eeprom_offsets::TOOL_COUNT, 1) == 1);
}


// reset the settings that can be changed via the onboard UI to defaults
void setDefaultSettings(uint8_t save_now){
    
    /// write blink and buzz defaults
    setDefaultLedEffects(eeprom_offsets::LED_STRIP_SETTINGS, save_now);
    setDefaultBuzzEffects(eeprom_offsets::BUZZ_SETTINGS, save_now);
    setDefaultsPreheat(eeprom_offsets::PREHEAT_SETTINGS, save_now);
	eeprom_address(eeprom_offsets::FILAMENT_HELP_SETTINGS) = 1;
//   eeprom_write_byte((uint8_t*)eeprom_offsets::FILAMENT_HELP_SETTINGS, 1);
	if (save_now)
		save_to_flash();}

//
void storeToolheadToleranceDefaults(uint8_t save_now){
	
	// assume t0 to t1 distance is in specifications (0 steps tolerance error)
	uint32_t offsets[3] = {0,0,0};
	for (uint8_t i=0;i<3;i++){
		eeprom_address(eeprom_offsets::TOOLHEAD_OFFSET_SETTINGS+(i*4)) = offsets[i];
	}
//	eeprom_write_block((uint8_t*)&(offsets[0]),(uint8_t*)(eeprom_offsets::TOOLHEAD_OFFSET_SETTINGS), 12 );
		if (save_now)
		save_to_flash();
}

/// Initialize entire eeprom map, including factor-set settings
void fullResetEEPROM(uint8_t save_now) {
	
	// axis inversion settings
	uint8_t axis_invert = 0b10111; // invert XYBZ
	eeprom_address(eeprom_offsets::AXIS_INVERSION) = axis_invert;
//	eeprom_write_byte((uint8_t*)eeprom_offsets::AXIS_INVERSION, axis_invert);
	
	// tool count settings
	eeprom_address(eeprom_offsets::TOOL_COUNT) = 1;
//	eeprom_write_byte((uint8_t*)eeprom_offsets::TOOL_COUNT, 1);
	
	// toolhead offset defaults
	storeToolheadToleranceDefaults(save_now);
	
	factoryResetEEPROM(save_now);
	if (save_now)
		save_to_flash();
}

}
