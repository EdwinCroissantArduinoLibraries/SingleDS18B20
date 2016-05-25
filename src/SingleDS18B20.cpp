#include "SingleDS18B20.h"
#include "Arduino.h"

SingleDS18B20::SingleDS18B20(OneWire* _oneWire) {
	// constructor
    _wire = _oneWire;
}

bool SingleDS18B20::setResolution(resolution newResolution) {
    // set resolution of the device to 9, 10, 11, or 12 bits
    // return false on failure
	// write the new resolution in the scratchpad
	_scratchpad[configuration] = newResolution;
	return _writeScratch();
}

bool SingleDS18B20::convert(void) {
	// issues the convert command to initiate a temperature conversion
	// return false on failure
	if (_wire->reset() == 0) return false;
	_wire->skip();
	_wire->write(startConversion);
	return true;
}

bool SingleDS18B20::read(void) {
    // attempt to read the scratchpad
	// return false on failure
	if (_wire->reset() == 0) return false;
	_wire->skip();
	_wire->write(readScratchpad);
	// Read all registers in a simple loop
	for (uint8_t i = 0; i < 9; i++) {
		_scratchpad[i] = _wire->read();
	}
	// check the CRC
	if (_wire->crc8(_scratchpad, 8) != _scratchpad[scratchpadCRC])
		return false;
	return true;
}

SingleDS18B20::resolution SingleDS18B20::getResolution(void) {
    // returns the resolution of the device
    // needs a successful read first
	return resolution(_scratchpad[configuration]);
}

int16_t SingleDS18B20::getTempAsRaw(void) {
	// returns temperature raw value (12 bit integer of 1/16 degrees C)
	// needs a successful read first
	return (((int16_t) _scratchpad[temperatureHighByte]) << 8)
			| ((int16_t) _scratchpad[temperatureLowByte]);
}

float SingleDS18B20::getTempAsC(void) {
	// returns the temperature as float in degrees Celsius
    // needs a successful read first
	return float(getTempAsRaw()) / 16;
}

int16_t SingleDS18B20::getUserData(void) {
	// returns the userdata
    // needs a successful read first
	return (((int16_t) _scratchpad[temperatureHighAlarm]) << 8)
			| ((int16_t) _scratchpad[temperatureLowAlarm]);
}

bool SingleDS18B20::setUserData(int16_t userData) {
	// set the userdata
	// return false on failure
	_scratchpad[temperatureHighAlarm] = userData >> 8;
	_scratchpad[temperatureLowAlarm] = userData & 255;
	return _writeScratch();
}

bool SingleDS18B20::copy(void) {
    // copy the resolution and the
    // userdata in the eeprom of the device.
	// return false on failure
	if (_wire->reset() == 0) return false;
	_wire->skip();
	_wire->write(copyScratchpad);
	delay(10); // time required to write into the eeprom
	return true;
}

bool SingleDS18B20::_writeScratch(void) {
	if (_wire->reset() == 0) return false;
	_wire->skip();
	_wire->write(writeScratchpad);
	_wire->write(_scratchpad[temperatureHighAlarm]); // high alarm temp
	_wire->write(_scratchpad[temperatureLowAlarm]); // low alarm temp
	_wire->write(_scratchpad[configuration]);
	return true;
}

bool SingleDS18B20::recall(void) {
    // recall the resolution and the
    // userdata from the eeprom of the device.
	// return false on failure
	if (_wire->reset() == 0) return false;
	_wire->skip();
	_wire->write(recallScratchpad);
	delay(10); // time required to read from the eeprom (not confirmed but better safe then sorry)
	return true;
}
