/*
ClockDigit.h - Class to manage digits on clock face
*/

#ifndef _CLOCKDIGIT_H_
#define _CLOCKDIGIT_H_

#include <SPI.h>
#include <RA8875.h>

// Font size for digits
#define HEXFONTSIZE 2
#define BINFONTSIZE 3
#define SETUPFONTSIZE 0

// Define whether a full or partial refresh is needed
#define REFRESH_MIN 0
#define REFRESH_ALL 1

// Definitions for high & low segments for a variety of uses
#define HRHIGH	0	// Hour high bits
#define HRLOW	1	// Hour low bits
#define MNHIGH	2	// Minutes high bits
#define MNLOW	3	// Minutes low bits
#define SEHIGH	4	// Seconds high bits
#define SELOW	5	// Seconds low bits
#define DYHIGH	6	// Day high bits
#define DYLOW	7	// Day low bits
#define MOHIGH	8	// Month high bits
#define MOLOW	9	// Month low bits
#define YRHIGH	10	// Year high bits
#define YRLOW	11	// Year low bits
#define IDNULL  12	// NO Value - Placeholder

class ClockDigit
{
public:
	ClockDigit();
	~ClockDigit();
	void setup(char c, int w, int h, uint16_t x, uint16_t y, uint8_t u);
	void eraseChar(RA8875 *disp, uint16_t bgColor);
	void drawChar(RA8875 *disp, uint16_t fg, uint16_t bg);
	int drawBinary(RA8875 *disp, uint16_t fg, uint16_t bg);
	bool setNewChar(uint8_t t, char c, int mode = REFRESH_MIN);
	void triggerHexUpdate() { updatedHex = true; }
	void triggerBinaryUpdate() { updatedBinary = true; }

private:
	int16_t currx, curry;		// Current X/Y position on the display
	char dChar, oldDChar;		// The character we're representing, and the last character 
	uint8_t tVal; // The value of the character we're representing
	bool updatedHex, updatedBinary;  // Has this been updated? If so, need to redraw
	uint8_t uType; // Unit for this character (Hour, Minute, Month, etc)
};

class AmPmDot
{
public:
	AmPmDot();
	~AmPmDot();
	void setup(uint8_t x, uint8_t y, uint8_t r);
	void eraseDot(RA8875 *disp, uint16_t color);
	void drawDot(RA8875 *disp, uint16_t color);
	void refreshDot(RA8875 *disp, uint16_t color);
	bool isOn() { return on; }

private:
	bool on;
	uint8_t xLoc, yLoc, radius;
};


#endif //_CLOCKDIGIT_H_
