/*
ClockDisplay.cpp
Manage the display of the LCD clock face
*/

#include "ClockDisplay.h"
#include "EEPROMFunctions.h"
#include "RTClock.h"
#include "Button.h"

extern RTClockClass RTClock;	// Real-time clock object
Button buttonArray[MAXBUTTONS];	// Array of buttons used on the configuration screen

/**************************************************************************
@brief  Converts raw touch screen locations (screenPtr) into actual pixel locations on the display (displayPtr) using the
supplied matrix.

@param[out] displayPtr  Pointer to the tsPoint_t object that will hold
the compensated pixel location on the display
@param[in]  screenPtr   Pointer to the tsPoint_t object that contains the
raw touch screen co-ordinates (before the
calibration calculations are made)
@param[in]  matrixPtr   Pointer to the calibration matrix coefficients
used during the calibration process (calculated
via the tsCalibrate() helper function)
**************************************************************************/
int ClockDisplay::calibrateTSPoint(tsPoint_t * displayPtr, tsPoint_t * screenPtr )
{
	//tsMatrix_t * matrixPtr = &_tsMatrix;
	int  retValue = 0;

	if (_tsMatrixPtr->Divider != 0)
	{
		displayPtr->x = ((_tsMatrixPtr->An * screenPtr->x) +
			(_tsMatrixPtr->Bn * screenPtr->y) +
			_tsMatrixPtr->Cn
			) / _tsMatrixPtr->Divider;

		displayPtr->y = ((_tsMatrixPtr->Dn * screenPtr->x) +
			(_tsMatrixPtr->En * screenPtr->y) +
			_tsMatrixPtr->Fn
			) / _tsMatrixPtr->Divider;
	}
	else
	{
		return -1;
	}

	return(retValue);
}

/**************************************************************************/
/*!
@brief  Checks for a touch event
*/
/**************************************************************************/
int ClockDisplay::checkForTouchEvent(RA8875* disp, tsPoint_t * point, bool waitMode)
{

	tsPoint_t raw, calibrated;
	uint16_t x, y;

	disp->touchEnable(true);
	if (disp->touched())
	{
		// We're reading the raw register data.
		// The Sumotoy calibration routine was highly inaccurate,
		// So we're using the Adafruit calibration code instead
		disp->touchReadAdc(&x, &y);	
		raw.x = x;
		raw.y = y;
		calibrateTSPoint(&calibrated, &raw);
		point->x = calibrated.x;
		point->y = calibrated.y;
		//disp->fillCircle(point->x, point->y, 3, RA8875_YELLOW);

		if (waitMode) // waitMode = wait for remaining touches to be cleared
		{
			while (disp->touched())
			{
				disp->touchReadAdc(&x, &y);
				delay(1);
			}
		}
		return 1;
	}
	else
	{
		point->x = 0;
		point->y = 0;
		return 0;
	}
	return 0;
}

/**************************************************************************/
/*!
@brief  Starts the screen calibration process.  Each corner will be
tested, meaning that each boundary (top, left, right and
bottom) will be tested twice and the readings averaged.
*/
/**************************************************************************/
void ClockDisplay::tsCalibrate(RA8875* disp)
{
	// read calibration data from eeprom
	_tsMatrix0.An = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION);
	_tsMatrix0.Bn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 4);
	_tsMatrix0.Cn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 8);
	_tsMatrix0.Dn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 12);
	_tsMatrix0.En = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 16);
	_tsMatrix0.Fn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 20);
	_tsMatrix0.Divider = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 24);

	/*
	Serial.println(_tsMatrix0.An);
	Serial.println(_tsMatrix0.Bn);
	Serial.println(_tsMatrix0.Cn);
	Serial.println(_tsMatrix0.Dn);
	Serial.println(_tsMatrix0.En);
	Serial.println(_tsMatrix0.Fn);
	Serial.println(_tsMatrix0.Divider);
	*/

	_tsMatrix180.An = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 28);
	_tsMatrix180.Bn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 32);
	_tsMatrix180.Cn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 36);
	_tsMatrix180.Dn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 40);
	_tsMatrix180.En = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 44);
	_tsMatrix180.Fn = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 48);
	_tsMatrix180.Divider = EEPROMReadSignedLong(EEPROM_CALIBRATION_LOCATION + 52);

	/*
	Serial.println(_tsMatrix180.An);
	Serial.println(_tsMatrix180.Bn);
	Serial.println(_tsMatrix180.Cn);
	Serial.println(_tsMatrix180.Dn);
	Serial.println(_tsMatrix180.En);
	Serial.println(_tsMatrix180.Fn);
	Serial.println(_tsMatrix180.Divider);
	*/
}

// Display boot-up colors test pattern on the screen. Oooh, pretty!
void ClockDisplay::testPattern(RA8875 *disp)
{
	uint16_t colors[] = { RA8875_WHITE , RA8875_RED , RA8875_YELLOW , RA8875_GREEN , RA8875_CYAN , RA8875_MAGENTA, RA8875_BLACK };
	for (int i = 0; i < 7; ++i)
	{
		disp->fillWindow(colors[i]);
		delay(500);
	}
}

ClockDisplay::ClockDisplay()
{
	// Initiatize Main Time section
	timeArray[0].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_TIMEHOURHIGH, Y_TIME_UPPER, HRHIGH);
	timeArray[1].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_TIMEHOURLOW, Y_TIME_UPPER, HRLOW);
	timeArray[2].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_TIMEMINUTEHIGH, Y_TIME_UPPER, MNHIGH);
	timeArray[3].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_TIMEMINUTELOW, Y_TIME_UPPER, MNLOW);
	timeArray[4].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_TIMESECONDHIGH, Y_TIME_UPPER, SEHIGH);
	timeArray[5].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_TIMESECONDLOW, Y_TIME_UPPER, SELOW);
	colonChar1.setup(':', W_COLON, H_COLON, X_COLON1, Y_TIME_UPPER, IDNULL);
	colonChar2.setup(':', W_COLON, H_COLON, X_COLON2, Y_TIME_UPPER, IDNULL);

	// Initialize Main Date section
	dateArray[0].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_DATEMONTHHIGH, Y_DATE_UPPER, MOHIGH);
	dateArray[1].setup('1', W_LARGEDIGIT, H_LARGEDIGIT, X_DATEMONTHLOW, Y_DATE_UPPER, MOLOW);
	dateArray[2].setup('0', W_LARGEDIGIT, H_LARGEDIGIT, X_DATEDAYHIGH, Y_DATE_UPPER, DYHIGH);
	dateArray[3].setup('1', W_LARGEDIGIT, H_LARGEDIGIT, X_DATEDAYLOW, Y_DATE_UPPER, DYLOW);
	dateArray[4].setup('1', W_LARGEDIGIT, H_LARGEDIGIT, X_DATEYEARHIGH, Y_DATE_UPPER, YRHIGH);
	dateArray[5].setup('6', W_LARGEDIGIT, H_LARGEDIGIT, X_DATEYEARLOW, Y_DATE_UPPER, YRLOW);
	slashChar1.setup('/', W_SLASH, H_SLASH, X_SLASH1, Y_DATE_UPPER, IDNULL);
	slashChar2.setup('/', W_SLASH, H_SLASH, X_SLASH2, Y_DATE_UPPER, IDNULL);

	// Initialize  up AM/PM indicator dots
	amDot.setup(X_AMPM, Y_AM, AMPM_DOTSIZE);
	pmDot.setup(X_AMPM, Y_PM, AMPM_DOTSIZE);

	numberBase = BASE_HEX;	// Default number base for display is HEX
	configMode = false;		// In "run" mode, not configutration mode
	displayBase = DISPLAY_24H;	// Default to military time display
	amPm = AMPM_MORNING;				// default to AM
	rotation = ROTATION_0;				// 0-degree screen rotation
	_tsMatrixPtr = &_tsMatrix0;			// Touch screen calibration matrix for 0-degree rotation
}


ClockDisplay::~ClockDisplay()
{
}

/*
Refresh the time on the clock
Refreshes only the internal time elements. Does NOT refresh the time on the clock's face. refreshClock() does that.
Mode is passed through to the function that updates the time/date character objects:
	REFRESH_MIN = refresh an object only if the value of the time element in that object has changed. Minimizes screen redraws, which can be slow on the tft display.
	REFRESH_ALL = Refresh the object whether it's changed or not. Forces a full redraw of the whole clock acreen.
*/
void ClockDisplay::refreshTime(RA8875 *disp, int mode)
{
	uint8_t tUnit;
	char charRep[3];
	int i;
	char *baseArray = "0123456789ABCDEF";	// Possible display digits

	for (i = UNIT_HOUR; i <= UNIT_YEAR_SHORT; ++i)	// Loop through all time/date parts
	{
		tUnit = RTClock.getUnit(i);

		if (i == UNIT_HOUR)		// Account for 12H display
		{
			amPm = (tUnit < 12) ? AMPM_MORNING : AMPM_AFTERNOON;
			if (displayBase == DISPLAY_12H) 
			{
				if (tUnit > 12)
					tUnit -= 12;
				if (tUnit == 0) 
					tUnit = 12;
			}
		}

		// Account for 12-hour display time
		// Get high- and low-order digits base on the current display mode (decimal or hex)
		// If in configuration mode mode, force decimal base.
		charRep[0] = baseArray[tUnit / (configMode ? BASE_DEC : numberBase)];	
		charRep[1] = baseArray[tUnit % (configMode ? BASE_DEC : numberBase)];	// Get low-order digit
		charRep[2] = '\0';	// NULL-terminate string, not because it's really needed here, but just for safety

		timeArray[(i - 1) * 2].setNewChar((tUnit >> 4), charRep[0], mode);			// Set high-order digit & bits
		timeArray[((i - 1) * 2) + 1].setNewChar((tUnit & 0xF), charRep[1], mode);		// Set low-order digit & bits
	}

	return;
}

void ClockDisplay::setRotation(uint8_t rot)
{
	rotation = rot;
	_tsMatrixPtr = (rotation == ROTATION_0) ? &_tsMatrix0 : &_tsMatrix180;	// Set calibration matrix based on screen orientation (0 or 180 degrees)
}


/*
Refresh the clock face.
refreshMode indicates how to handles the refresh
	REFRESH_MIN = refresh the character object only if the value of the time element in that object has changed. Minimizes screen redraws, which can be slow on the tft display.
	REFRESH_ALL = Refresh the character whether it's changed or not. Forces a full redraw of the whole clock.
drawMode indicates what parts of the clock face to draw
	DRAW_HEXBIN	= Draw both Hex & Binary parts
	DRAW_HEXONLY = Draw only the upper hexadecimal part
*/
void ClockDisplay::refreshClock(RA8875 *disp, int refreshMode, int drawMode)
{
	int i;

	if (refreshMode == REFRESH_ALL)
		disp->fillWindow(bgColor);		// Start with a clean slate

	disp->setRotation(rotation);
		
	// Print time
	for (int i = 0; i < 6; ++i)		// HH, MM, SS
	{
		timeArray[i].drawChar(disp, fgColor, bgColor);

		// Normally we draw both the hex/decimal time on the upper part of the screen and the binary time on the lower part of the screen. 
		// On the configuration screen, we don't want to draw the binary time, because that's where the config buttons go
		// DRAW_HEXONLY indicates to not draw the lower binary part
		if (drawMode == DRAW_HEXBIN) 
			timeArray[i].drawBinary(disp, fgColor, bgColor);
	}
	colonChar1.drawChar(disp, fgColor, bgColor);	// 2 colons between time elements
	colonChar2.drawChar(disp, fgColor, bgColor);

	// Print Date
	for (int i = 0; i<6; ++i)		// MM, DD, YY
	{
		dateArray[i].drawChar(disp, fgColor, bgColor);

		// Same as above
		if (drawMode == DRAW_HEXBIN)
			dateArray[i].drawBinary(disp, fgColor, bgColor);
	}
	slashChar1.drawChar(disp, fgColor, bgColor);	// 2 slashes between date elements
	slashChar2.drawChar(disp, fgColor, bgColor);

	if (displayBase == DISPLAY_12H)	// Need AM/PM indicator
	{
		if (amPm == AMPM_MORNING)
		{
			if (refreshMode == REFRESH_ALL)
				amDot.refreshDot(disp, fgColor);
			else
				amDot.drawDot(disp, fgColor);
			pmDot.eraseDot(disp, bgColor);
		}
		else // AMPM_AFTERNOON
		{
			amDot.eraseDot(disp, bgColor);
			if (refreshMode == REFRESH_ALL)
				pmDot.refreshDot(disp, fgColor);
			else
				pmDot.drawDot(disp, fgColor);
		}
	}
	else
	{
		amDot.eraseDot(disp, bgColor);
		pmDot.eraseDot(disp, bgColor);
	}

	if ((refreshMode == REFRESH_ALL) && (drawMode == DRAW_HEXBIN))  // Refreshing whole screen (hex & binary). Need to add the binary labels
	{
		disp->setFont(INT);
		disp->setTextColor(fgColor, bgColor);
		disp->setFontScale(BINFONTSIZE);

		disp->setCursor(X_BIN_TIMELABEL, Y_BIN_1); disp->println(F("H:"));
		disp->setCursor(X_BIN_TIMELABEL, Y_BIN_2); disp->println(F("M:"));
		disp->setCursor(X_BIN_TIMELABEL, Y_BIN_3); disp->println(F("S:"));

		disp->setCursor(X_BIN_DATELABEL, Y_BIN_1); disp->println(F("M:"));
		disp->setCursor(X_BIN_DATELABEL, Y_BIN_2); disp->println(F("D:"));
		disp->setCursor(X_BIN_DATELABEL, Y_BIN_3); disp->println(F("Y:"));
	}

	return;
}


/*
Given an X,Y point on the screen, return which button (if any) was pressed
Returns 0-MAXBUTTONS for an actual button
Returns -1 if area pressed was not a button
*/
int ClockDisplay::identifyArea(tsPoint_t point)
{
	int i;

	for (i = 0; i < MAXBUTTONS; ++i)
	{
		if (buttonArray[i].isButton(point.x, point.y))
			return i;
	}
	return -1;
}

/*
Displays the setup screen, places the buttons, and watches for screen presses
*/
int ClockDisplay::setupScreen(RA8875* disp)
{
	tsPoint_t calibrated;	// Holds calibrated screen points when screen is touched
	int touchArea;			// The button that was touched
	int i, newRefreshMode;
	bool redrawButtons = true, exitLoop=false;
	uint16_t newFg=fgColor, newBg=bgColor;
	
	/*
	systemresetCounter is used as a safeguard to accidentally resetting the clock
	The user must press the three reset buttons in order to force the reset.
	Press the buttons in any other order and systemresetCounter is reset to zero and the user must start again.
	*/
	int systemResetCounter = 0;

	configMode = true;  // Tells the clock we're in configuration mode. Suppress some normal screen drawing functions

	// Initialize all the buttons on the display
	// These are the buttons to adjust the time/date up & down
	buttonArray[BTN_HOURUP].setup(X_TIMEHOURHIGH, Y_TIME_UPPER, ((X_COLON1) - (X_TIMEHOURHIGH)), (H_LARGEDIGIT/2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_HOURDOWN].setup(X_TIMEHOURHIGH, Y_TIME_MID, ((X_COLON1) - (X_TIMEHOURHIGH)), (H_LARGEDIGIT/2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_MINUTEUP].setup(X_TIMEMINUTEHIGH, Y_TIME_UPPER, ((X_COLON2) - (X_TIMEMINUTEHIGH)), (H_LARGEDIGIT/2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_MINUTEDOWN].setup(X_TIMEMINUTEHIGH, Y_TIME_MID, ((X_COLON2) - (X_TIMEMINUTEHIGH)), (H_LARGEDIGIT / 2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_MONTHUP].setup(X_DATEMONTHHIGH, Y_DATE_UPPER, ((X_SLASH1)-(X_DATEMONTHHIGH)), (H_LARGEDIGIT / 2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_MONTHDOWN].setup(X_DATEMONTHHIGH, Y_DATE_MID, ((X_SLASH1)-(X_DATEMONTHHIGH)), (H_LARGEDIGIT / 2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_DAYUP].setup(X_DATEDAYHIGH, Y_DATE_UPPER, ((X_SLASH2) - (X_DATEDAYHIGH)), (H_LARGEDIGIT / 2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_DAYDOWN].setup(X_DATEDAYHIGH, Y_DATE_MID, ((X_SLASH2) - (X_DATEDAYHIGH)), (H_LARGEDIGIT / 2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_YEARUP].setup(X_DATEYEARHIGH, Y_DATE_UPPER, ((X_DATEYEARHIGH) + (W_LARGEDIGIT * 2)), (H_LARGEDIGIT / 2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	buttonArray[BTN_YEARDOWN].setup(X_DATEYEARHIGH, Y_DATE_MID, ((X_DATEYEARHIGH) + (W_LARGEDIGIT * 2)), (H_LARGEDIGIT / 2), NULL_COLOR, NULL_COLOR, NULL_COLOR, NULL, SETUPFONTSIZE, NULL, NULL, NULL);
	
	// These are the configuration buttons (color, number base, reset, etc)
	buttonArray[BTN_FGBLACK].setup(X_COLOR1, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_BLACK, RA8875_WHITE, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_FGBLUE].setup(X_COLOR2, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_BLUE, RA8875_WHITE, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_FGRED].setup(X_COLOR3, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_RED, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_FGGREEN].setup(X_COLOR4, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_GREEN, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_FGCYAN].setup(X_COLOR5, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_CYAN, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_FGMAGENTA].setup(X_COLOR6, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_MAGENTA, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_FGYELLOW].setup(X_COLOR7, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_YELLOW, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_FGWHITE].setup(X_COLOR8, Y_COLOR_FG, W_COLOR, H_COLOR, RA8875_WHITE, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGBLACK].setup(X_COLOR1, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_BLACK, RA8875_WHITE, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGBLUE].setup(X_COLOR2, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_BLUE, RA8875_WHITE, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGRED].setup(X_COLOR3, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_RED, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGGREEN].setup(X_COLOR4, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_GREEN, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGCYAN].setup(X_COLOR5, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_CYAN, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGMAGENTA].setup(X_COLOR6, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_MAGENTA, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGYELLOW].setup(X_COLOR7, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_YELLOW, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BGWHITE].setup(X_COLOR8, Y_COLOR_BG, W_COLOR, H_COLOR, RA8875_WHITE, RA8875_BLACK, NULL, NULL, NULL, NULL, NULL, NULL);
	buttonArray[BTN_BASE].setup(X_BASE, Y_BASE, W_BASE, H_BASE, RA8875_WHITE, RA8875_BLACK, RA8875_BLACK, RA8875_WHITE, SETUPFONTSIZE, (numberBase == BASE_HEX)?"HEX":"DEC", (X_BASE + 12), (Y_BASE + 6));
	buttonArray[BTN_RST1].setup(X_RESET1, Y_RESET, W_RESET, H_RESET, RA8875_GREEN, RA8875_BLACK, RA8875_BLACK, RA8875_GREEN, SETUPFONTSIZE, "1st", (X_RESET1 + 7), (Y_RESET + 5));
	buttonArray[BTN_RST2].setup(X_RESET2, Y_RESET, W_RESET, H_RESET, RA8875_YELLOW, RA8875_BLACK, RA8875_BLACK, RA8875_YELLOW, SETUPFONTSIZE, "2nd", (X_RESET2 + 7), (Y_RESET + 5));
	buttonArray[BTN_RST3].setup(X_RESET3, Y_RESET, W_RESET, H_RESET, RA8875_RED, RA8875_BLACK, RA8875_WHITE, RA8875_RED, SETUPFONTSIZE, "3rd", (X_RESET3 + 7), (Y_RESET + 5));
	buttonArray[BTN_DONE].setup(X_DONE, Y_DONE, W_DONE, H_DONE, RA8875_RED, RA8875_BLACK, RA8875_WHITE, RA8875_RED, SETUPFONTSIZE+1, "Done!", (X_DONE + 7), (Y_DONE + 5));
	buttonArray[BTN_DISPLAY].setup(X_DISPLAY, Y_DISPLAY, W_DISPLAY, H_DISPLAY, RA8875_WHITE, RA8875_BLACK, RA8875_BLACK, RA8875_WHITE, SETUPFONTSIZE, (displayBase == DISPLAY_24H) ? "24H" : "12H", (X_DISPLAY + 12), (Y_DISPLAY + 6));
	buttonArray[BTN_ROTATE].setup(X_ROTATE, Y_ROTATE, W_ROTATE, H_ROTATE, RA8875_WHITE, RA8875_BLACK, RA8875_BLACK, RA8875_WHITE, SETUPFONTSIZE, "Rotate", (X_ROTATE + 3), (Y_ROTATE + 6));

	disp->fillWindow(bgColor);	// Clear the screen

	// Get the latest date & time
	refreshTime(disp, REFRESH_ALL);
	
	// Only draw upper part of screen
	refreshClock(disp, REFRESH_ALL, DRAW_HEXONLY);

	do
	{	
		if (redrawButtons)	// For some reason, we need to redraw the buttons and their labels
		{ 	
			disp->setFont(INT);
			for (i = 0; i < MAXBUTTONS; ++i)
				buttonArray[i].draw(disp, fgColor, bgColor);
			disp->setTextColor(RA8875_WHITE, RA8875_BLACK);
			disp->setFontScale(SETUPFONTSIZE);
			disp->setCursor(X_FOREBACKBASE_LABEL, Y_FORELABEL); disp->print(F(" Foreground:"));
			disp->setCursor(X_FOREBACKBASE_LABEL, Y_BACKLABEL); disp->print(F(" Background:"));
			disp->setCursor(X_FOREBACKBASE_LABEL, Y_BASELABEL); disp->print(F(" Number Base:"));
			disp->setCursor(X_DISPLAYLABEL, Y_DISPLAYLABEL); disp->print(F(" Display:"));
			disp->setCursor(X_RESETLABEL, Y_RESETLABEL); disp->print(F(" Reset:"));
			redrawButtons = false;
		}

		newRefreshMode = REFRESH_MIN;	// For any changes, refresh the screen only as much as needed
		if (checkForTouchEvent(disp, &calibrated, true) != 0)		// Was screen touched?
		{
			if ((touchArea = identifyArea(calibrated)) != -1)		// If so, was it touched in a button area?
			{
				switch (touchArea)
				{
				case BTN_HOURUP:	// Incremenmt hour
					RTClock.incrementUnit(UNIT_HOUR);
					systemResetCounter = 0;
					break;
				case BTN_HOURDOWN:	// Decrement hour
					RTClock.decrementUnit(UNIT_HOUR);
					systemResetCounter = 0;
					break;
				case BTN_MINUTEUP:	// Increment minute. Seconds are automatically set to zero
					RTClock.incrementUnit(UNIT_MINUTE);
					systemResetCounter = 0;
					break;
				case BTN_MINUTEDOWN:	// Decrement minute. Seconds are automatically set to zero
					RTClock.decrementUnit(UNIT_MINUTE);
					systemResetCounter = 0;
					break;
				case BTN_MONTHUP:		// Increment month
					RTClock.incrementUnit(UNIT_MONTH);
					systemResetCounter = 0;
					break;
				case BTN_MONTHDOWN:		// Decrement month
					RTClock.decrementUnit(UNIT_MONTH);
					systemResetCounter = 0;
					break;
				case BTN_DAYUP:			// Increment day
					RTClock.incrementUnit(UNIT_DAY);
					systemResetCounter = 0;
					break;
				case BTN_DAYDOWN:		// Decrement day
					RTClock.decrementUnit(UNIT_DAY);
					systemResetCounter = 0;
					break;
				case BTN_YEARUP:		// Increment year
					RTClock.incrementUnit(UNIT_YEAR);
					systemResetCounter = 0;
					break;
				case BTN_YEARDOWN:		// Decrement year
					RTClock.decrementUnit(UNIT_YEAR);
					systemResetCounter = 0;
					break;
				case BTN_FGBLACK:		// Set foreground to black
					newFg=RA8875_BLACK;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_FGBLUE:		// Set foreground to blue
					newFg = RA8875_BLUE;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_FGRED:			// Set foreground to red
					newFg = RA8875_RED;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_FGGREEN:		// Set foreground to green
					newFg = RA8875_GREEN;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_FGCYAN:		// Set foreground to cyan
					newFg = RA8875_CYAN;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_FGMAGENTA:		// Set foreground to magenta
					newFg = RA8875_MAGENTA;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_FGYELLOW:		// Set foreground to yellow
					newFg = RA8875_YELLOW;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_FGWHITE:		// Set foreground to white
					newFg = RA8875_WHITE;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGBLACK:		// Set background to black
					newBg = RA8875_BLACK;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGBLUE:		// Set background to blue
					newBg = RA8875_BLUE;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGRED:			// Set background to red
					newBg = RA8875_RED;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGGREEN:		// Set background to green
					newBg = RA8875_GREEN;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGCYAN:		// Set background to cyan
					newBg = RA8875_CYAN;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGMAGENTA:		// Set background to magenta
					newBg = RA8875_MAGENTA;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGYELLOW:		// Set background to yellow
					newBg = RA8875_YELLOW;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BGWHITE:		// Set background to white
					newBg = RA8875_WHITE;
					newRefreshMode = REFRESH_ALL;
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_BASE:			// Toggle between hex & decimal display
					numberBase = (numberBase == BASE_HEX) ? BASE_DEC : BASE_HEX;
					buttonArray[BTN_BASE].setLabel((numberBase == BASE_HEX) ? "HEX" : "DEC");
					redrawButtons = true;
					break;
				case BTN_RST1:			// Reset clock - 1st step
					/* 
					Resetting the clock resets the time, color, and display components back to default state, then reboots the Arduino.
					It does NOT reset the display calibration coordinates. To reset the calibration you must run the TftCalibration Sketch again.
					To reset the clock, the "1st", "2nd", and "3rd" buttons must be pressed in that order.
					This was done to prevent accidental reset due to the flaky nature of the touch screen accuracy.
					*/
					systemResetCounter = 1;
					break;	
				case BTN_RST2:			// Reset clock - second step
					if (systemResetCounter == 1)
						systemResetCounter = 2;
					else 
						systemResetCounter = 0;	// Start counting over again.
					break;
				case BTN_RST3:			// Reset clock - 3rd step
					if (systemResetCounter == 2)	// First two steps have already been completed
					{
						disp->fillWindow(RA8875_BLACK);	// Give immediate feedback to user
						EEPROMWritelong(EEPROM_SIGNATURE_LOCATION, (uint32_t)0x0);  // Reset system settings signature. Force new settings on reboot.
						RTClock.resetClock();		// Set time/date back to initial time
						softwareReset();
					}
					else
						systemResetCounter = 0;
					break;
				case BTN_DISPLAY:		// Toggle 12/24H display
					displayBase = (displayBase == DISPLAY_24H) ? DISPLAY_12H : DISPLAY_24H;
					buttonArray[BTN_DISPLAY].setLabel((displayBase == DISPLAY_24H) ? "24H" : "12H");
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_ROTATE:		// Rotate display 180 degrees
					setRotation((rotation == ROTATION_0) ? ROTATION_180 : ROTATION_0);		// screen Flip rotation 
					disp->setRotation(rotation);	// Reset screen rotation
					redrawButtons = true;
					systemResetCounter = 0;
					break;
				case BTN_DONE:
					newRefreshMode = REFRESH_ALL;
					disp->fillWindow(bgColor);
					exitLoop = true;
					break;
				}

			// Adjust screen to account for any color changes
			setFgColor(newFg);
			setBgColor(newBg);
			refreshTime(disp, newRefreshMode);
			refreshClock(disp, newRefreshMode, (exitLoop == false)?DRAW_HEXONLY:DRAW_HEXBIN);
			newRefreshMode = REFRESH_MIN;

			// Save new settings
			EEPROMWritelong(EEPROM_CONFIG_LOCATION, (uint32_t)fgColor);
			EEPROMWritelong(EEPROM_CONFIG_LOCATION + 4, (uint32_t)bgColor);
			EEPROMWritelong(EEPROM_CONFIG_LOCATION + 8, (uint32_t)numberBase);
			EEPROMWritelong(EEPROM_CONFIG_LOCATION + 12, (uint32_t)displayBase);
			EEPROMWritelong(EEPROM_CONFIG_LOCATION + 16, (uint32_t)rotation);
			}
		}
	}
	while (exitLoop == false);

	configMode = false;
	// Force redraw of colons & slashes, because the system doesn't think they've been updated and won't draw them otherwise.
	colonChar1.triggerHexUpdate();
	colonChar2.triggerHexUpdate();
	slashChar1.triggerHexUpdate();
	slashChar2.triggerHexUpdate();

	return 0;
}

void ClockDisplay::softwareReset() // Restarts program from beginning but does not reset the peripherals and registers
{
	asm volatile ("  jmp 0");
}

