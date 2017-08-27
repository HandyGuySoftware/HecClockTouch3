// RTClock.h

#ifndef _RTCLOCK_h
#define _RTCLOCK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// RTC Header Files
#include <Wire.h>
#include <ds3231.h>

// Define Time Structure Units
#define UNIT_NULL		0
#define UNIT_HOUR		1
#define UNIT_MINUTE		2
#define UNIT_SECOND		3
#define UNIT_MONTH		4
#define UNIT_DAY		5
#define UNIT_YEAR_SHORT	6
#define UNIT_YEAR		7

class RTClockClass
{
public:
	RTClockClass();
	~RTClockClass();
	void resetClock();
	void incrementUnit(uint8_t);
	void decrementUnit(uint8_t);
	uint8_t getUnit(uint8_t);

private:
	struct ts t; // RTC time structure
};

#endif  // _RTCLOCK_H

