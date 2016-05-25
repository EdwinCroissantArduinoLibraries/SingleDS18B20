#ifndef SingleDS18B20_h
#define SingleDS18B20_h
#define SingleDS18B20LIBVERSION "0.0.1"
/* Single DS18B20 library for the arduino microcontroller
 *
 * Copyright (C) 2016 Edwin Croissant
 *
 *  This program is free software: you can redistribute it and/or modify
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See the README.md file for additional information.
 */

#include <OneWire.h>

class SingleDS18B20 {
public:

	enum resolution {
		res9bit  = B00011111, 	//  9 bit, conversion takes max 93.75 ms
		res10bit = B00111111, 	// 10 bit, conversion takes max 187.5 ms
		res11bit = B01011111,	// 11 bit, conversion takes max 375 ms
		res12bit = B01111111 	// 12 bit, conversion takes max 750 ms
	};

	SingleDS18B20(OneWire*);

    bool setResolution(resolution);

    bool convert(void);

    bool read(void);

    resolution getResolution(void);

    int16_t getTempAsRaw(void);

    float getTempAsC(void);

    int16_t getUserData(void);

    bool setUserData(int16_t);

    bool copy(void);

    bool recall(void);

private:
    OneWire* _wire;

    uint8_t _scratchpad[9];

	enum commands {
		startConversion		= 0x44, // Tells device to take a temperature reading and put it on the scratchpad
		copyScratchpad		= 0x48, // Copy scratchpad to EEPROM
		readScratchpad		= 0xBE,	// Read scratchpad
		writeScratchpad		= 0x4E,	// Write scratchpad
		recallScratchpad	= 0xB8,	// Reload the scratchpad from EEPROM
	};

	enum scratchpadLocations {
		temperatureLowByte   = 0,
		temperatureHighByte  = 1,
		temperatureHighAlarm = 2,
		temperatureLowAlarm  = 3,
		configuration        = 4,
		scratchpadCRC        = 8
	};

    bool _writeScratch(void);
};

#endif // SingleDS18B20_h
