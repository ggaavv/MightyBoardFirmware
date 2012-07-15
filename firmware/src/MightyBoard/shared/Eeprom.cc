#include "Eeprom.hh"
#include "EepromMap.hh"

#include "Version.hh"
//#include <avr/eeprom.h>


namespace eeprom {

/**
 * if the EEPROM is initalized and matches firmware version, exit
 * if the EEPROM is not initalized, write defaults, and set a new version
 * if the EEPROM is initalized but is not the current version, re-write the version number
 */
void init() {
	read_all_from_flash();
	uint8_t prom_version[2];
	prom_version[0] = eeprom_address(eeprom_offsets::VERSION_LOW);
	prom_version[1] = eeprom_address(eeprom_offsets::VERSION_HIGH);
//	eeprom_read_block(prom_version,(const uint8_t*)eeprom_offsets::VERSION_LOW,2);
	if ((prom_version[1]*100+prom_version[0]) == firmware_version)
		return;

	/// if our eeprom is empty (version is still FF, ie unwritten)
	if (prom_version[1] == 0xff || prom_version[1] < 2) {
		fullResetEEPROM(0);
            //setDefaults();
	}

	//Update eeprom version # to match current firmware version
	prom_version[0] = firmware_version % 100;
	prom_version[1] = firmware_version / 100;
	for (uint8_t i=0; i<2;i++) {
		eeprom_address(eeprom_offsets::VERSION_LOW) = prom_version[i];
	}
//	eeprom_write_block(prom_version,(uint8_t*)eeprom_offsets::VERSION_LOW,2);
	//Update XHomeOffsets to update incorrect settings for single/dual machines
	setDefaultAxisHomePositions(0); //:FAR:Q: do we need to do this here?
	save_to_flash();
}

uint8_t getEeprom8(const uint16_t location, const uint8_t default_value) {
        uint8_t data;
        /// TODO: why not just use eeprom_read_byte?
    	data = *(uint32_t*)location;
    	if (data == 0xff) data = default_value;
//        eeprom_read_block(&data,(const uint8_t*)location,1);
//        if (data == 0xff) data = default_value;
        return data;
}

uint16_t getEeprom16(const uint16_t location, const uint16_t default_value) {
        uint16_t data;
        /// TODO: why not just use eeprom_read_word?
    	data = *(uint32_t*)location;
    	if (data == 0xffff) data = default_value;
//        eeprom_read_block(&data,(const uint8_t*)location,2);
//        if (data == 0xffff) data = default_value;
        return data;
}

uint32_t getEeprom32(const uint16_t location, const uint32_t default_value) {
	uint32_t data = *(uint32_t*)location;
//	uint32_t data = eeprom_read_dword((const uint32_t*)location);
        if (data == 0xffffffff) return default_value;
        return data;
}

float getEepromFixed32(const uint16_t location, const float default_value) {
	int32_t data = *(uint32_t*)location;
//        int32_t data = getEeprom32(location, 0xffffffff);
        if (data == 0xffffffff) return default_value;
        return ((float)data)/65536.0;
}


/// Fetch a fixed 16 value from eeprom
float getEepromFixed16(const uint16_t location, const float default_value) {
        uint8_t data[2];
        data[0] = *(uint32_t*)location;
        data[1] = *(uint32_t*)(location+1);
//        eeprom_read_block(data,(const uint8_t*)location,2);
        if (data[0] == 0xff && data[1] == 0xff) return default_value;
        return ((float)data[0]) + ((float)data[1])/256.0;
}


/// Write a fixed 16 value to eeprom
void setEepromFixed16(const uint16_t location, const float new_value)
{
    uint8_t data[2];
    data[0] = (uint8_t)new_value;
    data[1] = (int)((new_value - data[0])*256.0);
    eeprom_address(location) = data[0];
    eeprom_address(location+1) = data[1];
//    eeprom_write_block(data,(uint8_t*)location,2);
}


/// Write a fixed 16 value to eeprom
void setEepromFixed16(const uint16_t location, const float new_value, uint8_t save_now)
{
    uint8_t data[2];
    data[0] = (uint8_t)new_value;
    data[1] = (int)((new_value - data[0])*256.0);
    eeprom_address(location) = data[0];
    eeprom_address(location+1) = data[1];
//    eeprom_write_block(data,(uint8_t*)location,2);
}

} // namespace eeprom
