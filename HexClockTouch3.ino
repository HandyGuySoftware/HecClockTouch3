/*
 Name:		HexClockTouch3.ino
 Created:	11/25/2016 6:21:35 PM
 Author:	Stephen Fried
*/

#include <SPI.h>
#include <RA8875.h>
#include "EEPROMFunctions.h"
#include "ClockDisplay.h"
#include "RTClock.h"

// Definitions for RTC
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

RA8875 tft = RA8875(RA8875_CS, RA8875_RESET);  // 800x600 TFT Display 
RTClockClass RTClock;  // Real-time clock
ClockDisplay theClock;  // Clockface Object

unsigned long mTime1, mTime2;	// Millisecond time counters. Used to trap long-touch events

void setup() {
	struct ts t;

	Serial.begin(9600);

	//Serial.println("Begin setup()");
		
	/* Initialize the TFT display */
	tft.begin(Adafruit_800x480);
	tft.useINT(RA8875_INT);
	tft.touchBegin();
	tft.enableISR(true);

	// Initialize the RTC
	Wire.begin();
	DS3231_init(DS3231_INTCN);

	// Display boot-up test pattern
	theClock.testPattern(&tft);

	theClock.tsCalibrate(&tft);

	/*
	Check for EEPROM signature at location EEPROM_SIGNATURE_LOCATION.
	If you find Hex 30183018 (4 bytes) there, the clock has already been configured.
	If not, this is the first time the clock has been run since reset, so need to reconfigure screen & options
	*/
	uint32_t eepromSignature = EEPROMReadUnsignedLong(EEPROM_SIGNATURE_LOCATION);
	if (eepromSignature == EEPROM_SIGNATURE_VALUE)
	{
		// Read clock configuration settings
		theClock.setFgColor((uint16_t)EEPROMReadUnsignedLong(EEPROM_CONFIG_LOCATION));
		theClock.setBgColor((uint16_t)EEPROMReadUnsignedLong(EEPROM_CONFIG_LOCATION + 4));
		theClock.setBase((uint8_t)EEPROMReadUnsignedLong(EEPROM_CONFIG_LOCATION+8));
		theClock.setDisplayBase((uint8_t)EEPROMReadUnsignedLong(EEPROM_CONFIG_LOCATION + 12));
		theClock.setRotation((uint8_t)EEPROMReadUnsignedLong(EEPROM_CONFIG_LOCATION + 16));
		tft.setRotation(theClock.getRotation());
	}
	else
	{
		// Set default colors & number base for clock operation
		theClock.setFgColor(RA8875_WHITE);
		theClock.setBgColor(RA8875_BLACK);
		theClock.setBase(BASE_HEX);
		theClock.setDisplayBase(DISPLAY_24H);
		theClock.setRotation(ROTATION_180);
		
		// Save new configuration signature flag
		EEPROMWritelong(EEPROM_SIGNATURE_LOCATION, EEPROM_SIGNATURE_VALUE);

		// Save default clock configuration
		EEPROMWritelong(EEPROM_CONFIG_LOCATION, (uint32_t)RA8875_WHITE);		// Default foreground
		EEPROMWritelong(EEPROM_CONFIG_LOCATION + 4, (uint32_t)RA8875_BLACK);	// Default Background
		EEPROMWritelong(EEPROM_CONFIG_LOCATION + 8, (uint32_t)BASE_HEX);		// Default number base
		EEPROMWritelong(EEPROM_CONFIG_LOCATION + 12, (uint32_t)DISPLAY_24H);	// Default time display base
		EEPROMWritelong(EEPROM_CONFIG_LOCATION + 16, (uint32_t)ROTATION_180);			// Default screen rotation. Set to 180 degrees because clock design mounts the display upside-down.

		// Set initial time & preferences
		theClock.setupScreen(&tft);
	}

	// Refresh all time segments. 
	// The first time around, need to refresh all so they will all be displayed
	// For subsequent calls, only the segments that are new will be re-displayed
	theClock.refreshTime(&tft, REFRESH_ALL); 
	
	// Refresh the clock face. 
	// The first time around, need to refresh all so everything will be displayed
	// For subsequent calls, only the segments that are new will be re-displayed
	theClock.refreshClock(&tft, REFRESH_ALL);

	mTime1 = mTime2 = 0;

	//Serial.println("Ending setup()");

}

// the loop function runs over and over again until power down or reset
void loop() {

	tsPoint_t raw, calibrated;

	// See if screen is being touched
	if (theClock.checkForTouchEvent(&tft, &calibrated, false) != 0)
	{
		// This line useful for debugging. Places a yellow circle where the screen was touched
		//tft.fillCircle(calibrated.x, calibrated.y, 3, RA8875_YELLOW);
		
		// See if screen has been touched for more than 5 seonconds.
		// If so, enter setup mode
		if (mTime1 == 0)
			mTime1 = millis();	// Start 5 second count
		else
		{
			mTime2 = millis();	// Update 5 second count
			if ((mTime2 - mTime1) > 5000) // 5-second press
			{
				mTime1 = 0;
				theClock.setupScreen(&tft);
			}
		}
	}
	else
	{
		mTime1 = mTime2 = 0; // Reset 5 second count
	}

	// Get latest time info
	theClock.refreshTime(&tft);
	
	// Refresh clock face with new elements
	theClock.refreshClock(&tft);

	return;
}
