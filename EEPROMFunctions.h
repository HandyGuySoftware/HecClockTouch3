// EEPROMFunctions.h

#ifndef _EEPROMFUNCTIONS_h
#define _EEPROMFUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <EEPROM.h>

// The signature value indicates whether the panel has been calibrated or not
// If the value is at location EEPROM_SIGNATURE_LOCATION, the display has been calibrated
// If not, need to go through the calibration routine
#define EEPROM_SIGNATURE_VALUE		((uint32_t)0x30183018)

// EEPROM Data Locations
#define EEPROM_SIGNATURE_LOCATION	10	// Calibration signature value
#define EEPROM_CALIBRATION_LOCATION	100	// Calibration settings
#define EEPROM_CONFIG_LOCATION		200	//Clock configuration settings

extern void EEPROMWritelong(int address, uint32_t value);
extern uint32_t EEPROMReadUnsignedLong(int address);
extern int32_t EEPROMReadSignedLong(int address);

#endif // _EEPROMFUNCTIONS_h

