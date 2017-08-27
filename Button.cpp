#include "Button.h"
#include "BitMask.h""

Button::Button()
{
}

void Button::setup(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fill, uint16_t border, uint16_t txtFg, uint16_t txtBg, uint8_t tSize, char *lab, uint16_t labX, uint16_t labY)
{
	// Initialize first data area (data1 - 64-bit)
	updateSegment(&data1, x, 0x003FFFFFFFFFFFFF, 0x3FF, 54);  // setLocX
	updateSegment(&data1, y, 0xFFC00FFFFFFFFFFF, 0x3FF, 44);  // setLocY
	updateSegment(&data1, w, 0xFFFFF003FFFFFFFF, 0x3FF, 34);  // setWidth
	updateSegment(&data1, h, 0xFFFFFFFC00FFFFFF, 0x3FF, 24);  // setHeight
	updateSegment(&data1, labX, 0xFFFFFFFFFF003FFF, 0x3FF, 14);  // setLabelX
	updateSegment(&data1, labY, 0xFFFFFFFFFFFFC00F, 0x3FF, 4);  // setLabelY
	updateSegment(&data1, tSize, 0xFFFFFFFFFFFFFFF0, 0xF, 0);  // setTextSize

	// Initialize second data area (data2 - 16-bit)
	updateSegment(&data2, color2Bits(fill), 0x0FFF, 0xF, 12); // Update fill
	updateSegment(&data2, color2Bits(border), 0xF0FF, 0xF, 8); // Update Border
	updateSegment(&data2, color2Bits(txtFg), 0xFF0F, 0xF, 4); // Update text foreground
	updateSegment(&data2, color2Bits(txtBg), 0xFFF0, 0xF, 0); // Update text background
	label = lab;
}

Button::~Button()
{
}

void Button::draw(RA8875* disp, uint16_t txtFg, uint16_t txtBg)
{
	// Draw button as a rounded rectangle
	// Some buttons are transparent and are used only as input capture areas only.
	// These are the areas under the time/date numbers, which you can press but we don't want a button drawn over them
	// These next lines will draw the buttons, if needed
	if (getFill() != NULL_COLOR) disp->fillRoundRect(getLocX(), getLocY(), getWidth(), getHeight(), 5, getFill());		// Fill, if needed
	if (getBorder() != NULL_COLOR) disp->drawRoundRect(getLocX(), getLocY(), getWidth(), getHeight(), 5, getBorder());	// Border, if needed
	if (getTextFg() != NULL_COLOR)
		disp->setTextColor(getTextFg(), getTextBg());	// Saved text colors
	else
		disp->setTextColor(txtFg, txtBg);				// Default text colors
	
	// Draw label, if available
	if (label != NULL) 
	{
		disp->setCursor(getLabelX(), getLabelY());
		disp->setFontScale(getTextSize());
		disp->print(label);
	}
	return;
}

// Given x & y coordinates, indicate if the area pressed is for this button
bool Button::isButton(int x, int y)
{
	if ((x >= getLocX()) && (x <= (getLocX() + getWidth())) && (y >= getLocY()) && (y <= (getLocY() + getHeight())))
		return true;	// This is a button area
	else
		return false;  // Not a button area
}

/*
// Debug function to print the value of the 64-bit data space
// Uncomment if necessary
uint32_t Button::print64(uint64_t in)
{
	uint32_t tmp;
	tmp = (in & 0xffffffff00000000) >> 32;
	Serial.print(tmp); Serial.print("|");
	tmp = (in & 0xffffffff);
	Serial.print(tmp);
}
*/

// Convert from a 16-bit RA8875 color definition to a 4-bit HexClock color definition
uint8_t Button::color2Bits(uint16_t in)
{
	switch (in)
	{
	case RA8875_BLACK:
		return BIT_BLACK;
		break;
	case RA8875_BLUE:
		return BIT_BLUE;
		break;
	case RA8875_RED:
		return BIT_RED;
		break;
	case RA8875_GREEN:
		return BIT_GREEN;
		break;
	case RA8875_CYAN:
		return BIT_CYAN;
		break;
	case RA8875_MAGENTA:
		return BIT_MAGENTA;
		break;
	case RA8875_YELLOW:
		return BIT_YELLOW;
		break;
	case RA8875_WHITE:
		return BIT_WHITE;
		break;
	case NULL_COLOR:
		return BIT_NULL;
		break;
	}
}

// Convert from a 4-bit HexClock color definition to a 16-bit RA8875 color definition
uint16_t Button::bits2Color(uint8_t in)
{
	switch (in)
	{
	case BIT_NULL:
		return NULL_COLOR;
		break;
	case BIT_BLACK:
		return RA8875_BLACK;
		break;
	case BIT_BLUE:
		return RA8875_BLUE;
		break;
	case BIT_RED:
		return RA8875_RED;
		break;
	case BIT_GREEN:
		return RA8875_GREEN;
		break;
	case BIT_CYAN:
		return RA8875_CYAN;
		break;
	case BIT_MAGENTA:
		return RA8875_MAGENTA;
		break;
	case BIT_YELLOW:
		return RA8875_YELLOW;
		break;
	case BIT_WHITE:
		return RA8875_WHITE;
		break;
	}
}
