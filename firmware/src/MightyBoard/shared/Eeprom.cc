#include "Eeprom.hh"
#include "EepromMap.hh"

#include "Version.hh"
//#include <avr/eeprom.h>

extern "C" {
#include "comm.h"
}

namespace eeprom {

volatile bool flash_updated;

bool getflashupdate(void){
	return flash_updated;
}

void clearflashupdate(void){
	flash_updated = false;
}

/**
 * if the EEPROM is initalized and matches firmware version, exit
 * if the EEPROM is not initalized, write defaults, and set a new version
 * if the EEPROM is initalized but is not the current version, re-write the version number
 */
void init() {
	read_all_from_flash();
	uint8_t prom_version[2];
	prom_version[0] = getEeprom8(eeprom_offsets::VERSION_LOW, 0);
	prom_version[1] = getEeprom8(eeprom_offsets::VERSION_HIGH, 0);
//	eeprom_read_block(prom_version,(const uint8_t*)eeprom_offsets::VERSION_LOW,2);
	if ((prom_version[1]*100+prom_version[0]) == firmware_version)
		return;

	/// if our eeprom is empty (version is still FF, ie unwritten)
	if (prom_version[1] == 0xff || prom_version[1] < 2) {
		fullResetEEPROM();
            //setDefaults();
	}

	//Update eeprom version # to match current firmware version
	prom_version[0] = firmware_version % 100;
	prom_version[1] = firmware_version / 100;
	for (uint8_t i=0; i<2;i++) {
		setEeprom8(eeprom_offsets::VERSION_LOW + i, prom_version[i]);
	}
//	xprintf("%x" " (%s:%d)\n",eeprom_address(EEPROM_START_ADDRESS, eeprom_offsets::VERSION_LOW),_F_,_L_);
//	xprintf("%x" " (%s:%d)\n",eeprom_address(EEPROM_START_ADDRESS, eeprom_offsets::VERSION_HIGH),_F_,_L_);
//	eeprom_write_block(prom_version,(uint8_t*)eeprom_offsets::VERSION_LOW,2);
	//Update XHomeOffsets to update incorrect settings for single/dual machines
	setDefaultAxisHomePositions(); //:FAR:Q: do we need to do this here?
//	save_to_flash();
}

uint8_t getEeprom8(const uint16_t location, const uint8_t default_value) {
        uint8_t data;
        /// TODO: why not just use eeprom_read_byte?
		if (location < 0 || location >= eeprom_info::EEPROM_SIZE){
			xprintf("Start Address:%x Hex written:0x\%x char written:%c" "\n",EEPROM_START_ADDRESS + (location*4), (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location), (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location));
		}
    	data = (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location);
    	if (data == 0xff) data = default_value;
//        eeprom_read_block(&data,(const uint8_t*)location,1);
//        if (data == 0xff) data = default_value;
        return data;
}

void setEeprom8(const uint16_t location, const uint8_t set_value) {
        /// TODO: why not just use eeprom_read_byte?
		if (location < 0 || location >= eeprom_info::EEPROM_SIZE){
			xprintf("Start Address:%x Hex written:0x\%x char written:%c" "\n",EEPROM_START_ADDRESS + (location*4), (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location), (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location));
		}
    	eeprom_address(EEPROM_START_ADDRESS, location) = set_value;
    	flash_updated = true;
}

uint16_t getEeprom16(const uint16_t location, const uint16_t default_value) {
	uint16_t data =	getEeprom8(location, 0);
	data |= getEeprom8(location+1, 0)<<8;
        /// TODO: why not just use eeprom_read_word?
    	if (data == 0xffff) data = default_value;
//        eeprom_read_block(&data,(const uint8_t*)location,2);
//        if (data == 0xffff) data = default_value;
//    	xprintf("getEeprom16" " (%s:%d)\n",_F_,_L_);
//    	xprintf("%x" " (%s:%d)\n",data,_F_,_L_);
        return data;
}

void setEeprom16(const uint16_t location, const uint16_t set_value) {
	setEeprom8(location, set_value&0xff);
	setEeprom8(location+1, set_value>>8);
}

uint32_t getEeprom32(const uint16_t location, const uint32_t default_value) {
	uint32_t data =	getEeprom16(location, 0);
	data |= getEeprom8(location+2, 0)<<8;
//	uint32_t data = eeprom_read_dword((const uint32_t*)location);
        if (data == 0xffffffff) return default_value;
//    	xprintf("getEeprom32" " (%s:%d)\n",_F_,_L_);
//    	xprintf("%x" " (%s:%d)\n",data,_F_,_L_);
        return data;
}

void setEeprom32(const uint16_t location, const uint32_t set_value) {
	setEeprom16(location, (uint16_t)(set_value&0xffff));
	setEeprom16(location+2, (uint16_t)(set_value>>16));
}

float getEepromFixed32(const uint16_t location, const float default_value) {
	uint32_t data =	getEeprom16(location, 0);
	data |= getEeprom16(location+2, 0)<<16;

//	int32_t data = *(uint32_t*)(EEPROM_START_ADDRESS, location);
//        int32_t data = getEeprom32(location, 0xffffffff);
        if (data == 0xffffffff) return default_value;
//    	xprintf("getEepromFixed32" " (%s:%d)\n",_F_,_L_);
//    	xprintf("%x" " (%s:%d)\n",data,_F_,_L_);
//        return ((float)data)/65536.0;
     return (float)data;
}

void setEepromFixed32(const uint16_t location, const float set_value) {
	setEeprom16(location, (uint16_t)set_value&0xffff);
	setEeprom16(location+2, (uint16_t)set_value>>16);
}

/// Fetch a fixed 16 value from eeprom
float getEepromFixed16(const uint16_t location, const float default_value) {
//        uint8_t data[2];
		uint16_t data = (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location);
//        data[0] = (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location);
//        data[1] = (uint8_t)eeprom_address(EEPROM_START_ADDRESS, location + 1);
//        eeprom_read_block(data,(const uint8_t*)location,2);
//    	xprintf("getEepromFixed16" " (%s:%d)\n",_F_,_L_);
//    	xprintf("%x" " (%s:%d)\n",data[0],_F_,_L_);
//    	xprintf("%x" " (%s:%d)\n",data[1],_F_,_L_);
        if (data == 0xffff) return default_value;
        return ((float)data);
//        if (data[0] == 0xff && data[1] == 0xff) return default_value;
//        return ((float)data[0]) + ((float)data[1])/256.0;
}

} // namespace eeprom
