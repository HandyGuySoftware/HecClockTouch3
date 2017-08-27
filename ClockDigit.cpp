#include "ClockDigit.h"
#include "ClockDisplay.h"

// Font definition file
#include "MSTahomaBold48.c"

#include "ClockDisplay.h" // Need for number base definitions

ClockDigit::ClockDigit()
{
}

/*
Initialize character
c = char being displayed
w,h - width, height of character
x, y - X/Y coordinates for lower left corner of the digit
u - unit type
*/
void ClockDigit::setup(char c, int w, int h, uint16_t x, uint16_t y, uint8_t u)
{
	dChar = c;
	oldDChar = '\0';
	updatedHex = updatedBinary = true;
	tVal = 0;
	uType = u;
	currx = x;
	curry = y;
}

ClockDigit::~ClockDigit()
{
}

// Custom fonts don't automatically erase when overwritten. Need to help it along
void ClockDigit::eraseChar(RA8875 *disp, uint16_t bgColor)
{
	if (oldDChar != '\0')
	{
		// Set cursor, color, & print character
		disp->setCursor(currx, curry);
		disp->setTextColor(bgColor, bgColor);		// "erase" character by printing the character in the background color
		disp->print(oldDChar);
	}
	return;
}

// Update the binary display at the bottom of the screen
int ClockDigit::drawBinary(RA8875 *disp, uint16_t fg, uint16_t bg)
{
	char binChars[5];
	int i;
	int16_t x, y;

	// Only draw if character has been updated, because need to erase old one first
	if (updatedBinary == false)
		return;

	// Create a null-terminated string representation of the binary number
	for (i = 0; i < 4; ++i)
	{
		if (tVal & (0x1 << i))
			binChars[3 - i] = '1';
		else
			binChars[3 - i] = '0';
	}
	binChars[4] = '\0';

	// Prepare to print
	disp->setFont(INT);  // Reset to default (small) font for binary display
	disp->setFontScale(BINFONTSIZE);
	disp->setTextColor(fg, bg);

	// Set cursur to appropriate location
	switch (uType) {
	case HRHIGH:
		x = X_BINTIME + X_BIN_OFFSET_HIGH;
		y = Y_BIN_1;
		break;
	case HRLOW:
		x = X_BINTIME + X_BIN_OFFSET_LOW;
		y = Y_BIN_1;
		break;
	case MNHIGH:
		x = X_BINTIME + X_BIN_OFFSET_HIGH;
		y = Y_BIN_2;
		break;
	case MNLOW:
		x = X_BINTIME + X_BIN_OFFSET_LOW;
		y = Y_BIN_2;
		break;
	case SEHIGH:
		x = X_BINTIME + X_BIN_OFFSET_HIGH;
		y = Y_BIN_3;
		break;
	case SELOW:
		x = X_BINTIME + X_BIN_OFFSET_LOW;
		y = Y_BIN_3;
		break;
	case MOHIGH:
		x = X_BINDATE + X_BIN_OFFSET_HIGH;
		y = Y_BIN_1;
		break;
	case MOLOW:
		x = X_BINDATE + X_BIN_OFFSET_LOW;
		y = Y_BIN_1;
		break;
	case DYHIGH:
		x = X_BINDATE + X_BIN_OFFSET_HIGH;
		y = Y_BIN_2;
		break;
	case DYLOW:
		x = X_BINDATE + X_BIN_OFFSET_LOW;
		y = Y_BIN_2;
		break;
	case YRHIGH:
		x = X_BINDATE + X_BIN_OFFSET_HIGH;
		y = Y_BIN_3;
		break;
	case YRLOW:
		x = X_BINDATE + X_BIN_OFFSET_LOW;
		y = Y_BIN_3;
		break;
	}

	disp->setCursor(x, y);
	disp->print(binChars);
	updatedBinary = false; // reset updated flag to prevent unnecessary redrawing

	return;
}

/* 
Draw the digit character on the screen
fg, bg - foreground & backgrond color for the digit
*/
void ClockDigit::drawChar(RA8875 *disp, uint16_t fg, uint16_t bg)
{
	// Only draw if character has been updated, because we need to erase old one first
	if (updatedHex == false)
		return;

	disp->setFont(&MSTahomaBold48);	// Set to large font
	disp->setFontScale(HEXFONTSIZE);
	if (oldDChar != '\0')
		eraseChar(disp, bg); // erase the char currently at that location
	
	// Set cursor, color, & print character
	disp->setCursor(currx, curry);
	disp->setTextColor(fg, bg);
	disp->print(dChar);
	
	updatedHex = false; // reset updated flag to prevent unnecessary redrawing

	return;
}

// Update the character stored in the digit
bool ClockDigit::setNewChar(uint8_t t, char c, int mode)
{
	updatedHex = updatedBinary = false;	// Begin with the assumption that nothing has changed.

	// t is the value of the digit. 'c' is the character
	if ((c == dChar) && (t == tVal) && (mode != REFRESH_ALL)) // Same character & binary, no forced update
	{
		updatedHex = updatedBinary = false;
		return false;
	}
	
	if ((c != dChar) || (mode == REFRESH_ALL))
	{
		// New char. Make some changes
		oldDChar = dChar;	// Need to save the last character so we can erase it
		dChar = c;			// The new character
		updatedHex = true;
	}

	if ((t != tVal) || (mode == REFRESH_ALL))
	{
		tVal = t;			// Value of the unit
		updatedBinary = true;
	}

	return (updatedHex || updatedBinary);	// Something has changed
}


AmPmDot::AmPmDot()
{
}

AmPmDot::~AmPmDot()
{
}

void AmPmDot::setup(uint8_t x, uint8_t y, uint8_t r)
{
	xLoc = x;
	yLoc = y;
	radius = r;
	on = false;
}

void AmPmDot::eraseDot(RA8875 *disp, uint16_t color)
{
	if (on)	// Only erase if already displayed - eliminates flicker
	{
		disp->drawCircle(xLoc, yLoc, radius, color);
		disp->fillCircle(xLoc, yLoc, radius, color);
		on = false;
	}
}

void AmPmDot::drawDot(RA8875 *disp, uint16_t color)
{
	if (!on) // Only draw if already erased - eliminates flicker
	{
		disp->drawCircle(xLoc, yLoc, radius, color);
		disp->fillCircle(xLoc, yLoc, radius, color);
		on = true;
	}
}

void AmPmDot::refreshDot(RA8875 *disp, uint16_t color)
{
	if (on)
	{
		disp->drawCircle(xLoc, yLoc, radius, color);
		disp->fillCircle(xLoc, yLoc, radius, color);
	}
}


