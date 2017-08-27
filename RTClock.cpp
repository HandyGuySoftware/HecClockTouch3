// Wrapper class for DS3231 Real-Time clock module

#include "RTClock.h"

RTClockClass::RTClockClass()
{
}

RTClockClass::~RTClockClass()
{
}

// Initialize clock back to the beginning of time (00:00:00 1/1/2016)
void RTClockClass::resetClock()
{
	t.hour = t.min = t.sec = 0;
	t.mday = t.mon = 1;
	t.year = 2016;				// Set to 2016 so you don't have to wind up from 2000 when resetting clock
	t.year_s = 16;

	DS3231_set(t);	// Set updated time

	return;
}

// Get a unit of time from the RTC
uint8_t RTClockClass::getUnit(uint8_t unit)
{
	DS3231_get(&t); // Get updated time

	switch (unit)
	{
	case UNIT_HOUR:
		return t.hour;
		break;
	case UNIT_MINUTE:
		return t.min;
		break;
	case UNIT_SECOND:
		return t.sec;
		break;
	case UNIT_DAY:
		return t.mday;
		break;
	case UNIT_MONTH:
		return t.mon;
		break;
	case UNIT_YEAR:
		return t.year;
		break;
	case UNIT_YEAR_SHORT:
		return t.year_s;
		break;
	}
}


// Define number of days in each month (ignoring February shenanigans)
uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// Increment a specific clock segment by 1 unit
void RTClockClass::incrementUnit(uint8_t unit)
{
	bool isLeap = false;	// account for leap year
	
	switch (unit)
	{
	case UNIT_HOUR:
		if (++t.hour == 24)
			t.hour = 0;
		break;
	case UNIT_MINUTE:
		if (++t.min == 60)
			t.min = 0;
		t.sec = 0;
		break;
	case UNIT_SECOND:
		++t.sec;
		break;
	case UNIT_DAY:
		++t.mday;
		switch (t.mon) // See which month you're incrementing, adjust accordingly
		{
			case 1: if (t.mday >= 32) t.mday = 1; break;
			case 2: // Leap year evil-ness
				if ((t.year % 4 == 0) && !(t.year % 100 == 0) || (t.year % 400 == 0))
					isLeap = true;
				if ((t.mday >= 29) && !isLeap) // Normal Year
					t.mday = 1;
				else if (t.mday >= 30) // Leap year
					t.mday = 1;
				break;
			case 3: if (t.mday >= 32) t.mday = 1; break;
			case 4: if (t.mday >= 31) t.mday = 1; break;
			case 5: if (t.mday >= 32) t.mday = 1; break;
			case 6: if (t.mday >= 31) t.mday = 1; break;
			case 7: if (t.mday >= 32) t.mday = 1; break;
			case 8: if (t.mday >= 32) t.mday = 1; break;
			case 9: if (t.mday >= 31) t.mday = 1; break;
			case 10: if (t.mday >= 32) t.mday = 1; break;
			case 11: if (t.mday >= 31) t.mday = 1; break;
			case 12: if (t.mday >= 32) t.mday = 1; break;
		}
		break;
	case UNIT_MONTH:
		if (++t.mon == 13)
			t.mon = 1;
		if (t.mday > monthDays[t.mon - 1])  // Day is set higher than new month will allow
			t.mday = monthDays[t.mon - 1];
		break;
	case UNIT_YEAR:
		++t.year;
		++t.year_s;
		break;
	}

	DS3231_set(t);		// Update new time
}

// Decrement a specific clock segment by 1 unit
void RTClockClass::decrementUnit(uint8_t unit)
{
	switch (unit)
	{
	case UNIT_HOUR:
		if (t.hour == 0) t.hour = 23; else --t.hour;
		break;
	case UNIT_MINUTE:
		if (t.min == 0)	t.min = 59; else --t.min;
		t.sec = 0;
		break;
	case UNIT_DAY:
		if (--t.mday == 0)
		{
			switch (t.mon) // See which month you're incrementing, adjust accordingly
			{
			case 1: t.mday = 31; break;
			case 2: // Leap year evil-ness
				if ((t.year % 4 == 0) && !(t.year % 100 == 0) || (t.year % 400 == 0))
					t.mday = 29;
				else t.mday = 28;
				break;
			case 3: t.mday = 31; break;
			case 4: t.mday = 30; break;
			case 5: t.mday = 31; break;
			case 6: t.mday = 30; break;
			case 7: t.mday = 31; break;
			case 8: t.mday = 31; break;
			case 9: t.mday = 30; break;
			case 10: t.mday = 31; break;
			case 11: t.mday = 30; break;
			case 12: t.mday = 31; break;
			}
		}
		break;
	case UNIT_MONTH:
		if (--t.mon == 0) t.mon = 12;
		break;
	case UNIT_YEAR:
		--t.year;
		--t.year_s;
		break;
	}

	DS3231_set(t);		// Update new time
}