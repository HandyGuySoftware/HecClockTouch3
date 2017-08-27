// ClockDisplay.h
// Manages the display of the TFT clock face

#ifndef _CLOCKDISPLAY_h
#define _CLOCKDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ClockDigit.h"

// Touch screen cal structs
typedef struct Point
{
	int32_t x;
	int32_t y;
} tsPoint_t;

typedef struct //Matrix
{
	int32_t An,
		Bn,
		Cn,
		Dn,
		En,
		Fn,
		Divider;
} tsMatrix_t;

// Definitions for connecting the RA8875 Board
// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 2
#define RA8875_CS 10
#define RA8875_RESET 9

// Definitions for digit width & height
#define W_LARGEDIGIT	96
#define H_LARGEDIGIT	105
#define W_COLON			21
#define H_COLON			75
#define W_SLASH			50
#define H_SLASH			102

// Large time row definitions
#define X_TIMEHOURHIGH		55												// High-order hour digit
#define X_TIMEHOURLOW		(X_TIMEHOURHIGH + W_LARGEDIGIT)					// Low-order hour digit
#define X_TIMEMINUTEHIGH	(X_TIMEHOURLOW + W_LARGEDIGIT + W_COLON + 30)	// High-order minute digit
#define X_TIMEMINUTELOW		(X_TIMEMINUTEHIGH + W_LARGEDIGIT)				// Low-order minute digit
#define X_TIMESECONDHIGH	(X_TIMEMINUTELOW +W_LARGEDIGIT + W_COLON + 30)	// High-order second digit
#define X_TIMESECONDLOW		(X_TIMESECONDHIGH + W_LARGEDIGIT)				// Low-order second digit
#define X_COLON1			(X_TIMEHOURLOW + W_LARGEDIGIT + 5)					// Hour:minute colon
#define X_COLON2			(X_TIMEMINUTELOW + W_LARGEDIGIT + 5)				// Minute:second colon
#define Y_TIME_UPPER		(Y_TIME_LOWER - H_LARGEDIGIT)					// Upper Y of time row
#define Y_TIME_MID			(Y_TIME_LOWER - (H_LARGEDIGIT / 2))				// Midpoint Y of time row
#define Y_TIME_LOWER		125												// Lower Y of time row

// Large date row definitions
#define X_DATEMONTHHIGH		25												// High-order month digit
#define X_DATEMONTHLOW		(X_DATEMONTHHIGH + W_LARGEDIGIT)				// Low-order monthdigit
#define X_DATEDAYHIGH		(X_DATEMONTHLOW + W_LARGEDIGIT + W_SLASH + 35)	// High-order day digit
#define X_DATEDAYLOW		(X_DATEDAYHIGH + W_LARGEDIGIT)					// Low-order day digit
#define X_DATEYEARHIGH		(X_DATEDAYLOW + W_LARGEDIGIT + W_SLASH + 35)	// High-order year digit
#define X_DATEYEARLOW		(X_DATEYEARHIGH + W_LARGEDIGIT)					// Low-order year digit
#define X_SLASH1			(X_DATEMONTHLOW + W_LARGEDIGIT + 5)					// Month/day slash
#define X_SLASH2			(X_DATEDAYLOW + W_LARGEDIGIT + 5)					// Day/year slash
#define Y_DATE_LOWER		260												// Lower Y of time row
#define Y_DATE_UPPER		(Y_DATE_LOWER - H_LARGEDIGIT)					// Upper Y of time row
#define Y_DATE_MID			(Y_DATE_LOWER - (H_LARGEDIGIT / 2))				// Midpoint Y of time row

// Binary row definitions
#define X_BINTIME			131		// Starting Column for Binary Time
#define X_BINDATE			478		// Starting Columns for Binary Date
#define Y_BIN_1				280		// First row of binary displays
#define Y_BIN_2				342		// Second row of binary displays
#define Y_BIN_3				404		// Third row of binary displays
#define X_BIN_OFFSET_HIGH	0
#define X_BIN_OFFSET_LOW	128
#define X_BIN_TIMELABEL		68		// Starting Column for Binary Time Label
#define X_BIN_DATELABEL		415		// Starting Column for Binary Date Label

// Defines if running initial calibration or if calibration has already been done
#define CALIBRATE_NEW 0
#define CALIBRATE_EXISTING 1

// Define digit drawing modes
#define DRAW_HEXBIN	0
#define DRAW_HEXONLY 1

// Number base definitions
#define BASE_DEC 10
#define BASE_HEX 16

// X, Y, W, H locations for  various buttons
#define X_COLOR1	175
#define X_COLOR2	250
#define X_COLOR3	325
#define X_COLOR4	400
#define X_COLOR5	475
#define X_COLOR6	550
#define X_COLOR7	625	
#define X_COLOR8	700
#define Y_COLOR_FG	290
#define Y_COLOR_BG	355
#define W_COLOR		50
#define H_COLOR		30
#define X_BASE		170
#define Y_BASE		420
#define W_BASE		50
#define H_BASE		30
#define X_RESET1	505
#define X_RESET2	555
#define X_RESET3	605
#define Y_RESET		420
#define W_RESET		40
#define H_RESET		30
#define X_DONE		660
#define Y_DONE		405
#define W_DONE		90
#define H_DONE		50
#define X_DISPLAY	310
#define Y_DISPLAY	420
#define W_DISPLAY	50
#define H_DISPLAY	30
#define X_ROTATE	372
#define Y_ROTATE	420
#define W_ROTATE	55
#define H_ROTATE	30


// X,Y coordinates for configuration button labels
#define X_FOREBACKBASE_LABEL	60
#define Y_FORELABEL				295
#define Y_BACKLABEL				360
#define Y_BASELABEL				425	
#define X_RESETLABEL			440
#define Y_RESETLABEL			425
#define X_DISPLAYLABEL			230
#define Y_DISPLAYLABEL			425

// X/Y Coordinates for AM/PM indicators
#define X_AMPM		(X_TIMEHOURHIGH - 20)
#define Y_AM		(Y_TIME_UPPER + ((Y_TIME_MID - Y_TIME_UPPER)/2))
#define Y_PM		(Y_TIME_MID + ((Y_TIME_LOWER - Y_TIME_MID)/2))
#define AMPM_DOTSIZE	7

// Definitions for display modes
#define DISPLAY_24H		true
#define DISPLAY_12H		false
#define AMPM_MORNING	true
#define AMPM_AFTERNOON	false

// Definitions for screen rotation
#define ROTATION_0		0
#define ROTATION_90		1
#define ROTATION_180	2
#define ROTATION_270	3

class ClockDisplay
{
public:
	ClockDisplay();
	~ClockDisplay();

	void refreshTime(RA8875*, int mode = REFRESH_MIN);
	void refreshClock(RA8875*, int rmode = REFRESH_MIN, int dmode = DRAW_HEXBIN);
	void testPattern(RA8875*);
	void tsCalibrate(RA8875* disp);
	int checkForTouchEvent(RA8875* disp, tsPoint_t * point, bool waitMode = true);
	int calibrateTSPoint(tsPoint_t * displayPtr, tsPoint_t * screenPtr);
	void setFgColor(uint16_t fore) { fgColor = fore; }
	void setBgColor(uint16_t back) { bgColor = back; }
	void setBase(uint8_t base) { numberBase = base; }
	void setRotation(uint8_t rot);
	int getRotation() { return rotation; }
	void setDisplayBase(uint8_t base) { displayBase = ((base & 0x11)? true: false); }
	int setupScreen(RA8875* disp);

private:
	ClockDigit timeArray[6], dateArray[6], colonChar1, colonChar2, slashChar1, slashChar2;	// The time & date digits on the clock face
	AmPmDot amDot, pmDot;
	uint16_t fgColor, bgColor;	// Default foreground & background color for the screen

	// Screen Point references used in screen calibration routines
	tsMatrix_t      _tsMatrix0, _tsMatrix180, *_tsMatrixPtr;		// Calibration matrices for 0- and 180-degree rotations

	uint8_t numberBase;	// Display time/date as Hex or Decimal
	bool configMode;	// Are we in configuration mode or normal operation?
	bool displayBase;	// DISPLAY_24H (true) or DISPLAY_12H (false)
	bool amPm;			// AMPM_MORNING or AMPM_AFTERNOON
	uint8_t rotation;

	int identifyArea(tsPoint_t point);
	void softwareReset(void); // Restarts program from beginning but does not reset the peripherals and registers

};

#endif  // _CLOCKDISPLAY_h