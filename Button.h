// Button.h
#pragma once

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SPI.h>
#include <RA8875.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Define 4-bit compressed color values
#define BIT_BLACK	0x0
#define BIT_BLUE	0x1
#define BIT_RED		0x2
#define BIT_GREEN	0x3
#define BIT_CYAN	0x4
#define BIT_MAGENTA	0x5
#define BIT_YELLOW	0x6
#define BIT_WHITE	0x7
#define BIT_NULL	0x8

// Define "Buttons" for clock configuratoin screen
#define MAXBUTTONS		33

#define BTN_HOURUP		0
#define BTN_HOURDOWN	1
#define BTN_MINUTEUP	2
#define BTN_MINUTEDOWN	3
#define BTN_MONTHUP		4
#define BTN_MONTHDOWN	5
#define BTN_DAYUP		6
#define BTN_DAYDOWN		7
#define BTN_YEARUP		8
#define BTN_YEARDOWN	9
#define BTN_FGBLACK		10
#define BTN_FGBLUE		11
#define BTN_FGRED		12
#define BTN_FGGREEN		13
#define BTN_FGCYAN		14
#define BTN_FGMAGENTA	15
#define BTN_FGYELLOW	16
#define BTN_FGWHITE		17
#define BTN_BGBLACK		18
#define BTN_BGBLUE		19
#define BTN_BGRED		20
#define BTN_BGGREEN		21
#define BTN_BGCYAN		22
#define BTN_BGMAGENTA	23
#define BTN_BGYELLOW	24
#define BTN_BGWHITE		25
#define BTN_BASE		26
#define BTN_RST1		27
#define BTN_RST2		28
#define BTN_RST3		29
#define BTN_DONE		30
#define BTN_DISPLAY		31
#define BTN_ROTATE		32

#define NULL_COLOR 0x1234

class Button
{
public:
	Button();
	~Button();
	void setup(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fill, uint16_t border, uint16_t txtFg, uint16_t txtBg, uint8_t tSize, char *lab, uint16_t labX, uint16_t labY);
	void draw(RA8875*, uint16_t txtFg, uint16_t txtBg);
	bool isButton(int x, int y);

	uint8_t color2Bits(uint16_t in);
	uint16_t bits2Color(uint8_t in);

	uint16_t getLocX() { return (uint16_t)((data1 >> 54) & 0x3ff); }
	uint16_t getLocY() { return (uint16_t)((data1 >> 44) & 0x3ff); }
	uint16_t getWidth() { return (uint16_t)((data1 >> 34) & 0x3ff); }
	uint16_t getHeight() { return (uint16_t)((data1 >> 24) & 0x3ff); }
	uint16_t getLabelX() { return (uint16_t)((data1 >> 14) & 0x3ff); }
	uint16_t getLabelY() { return (uint16_t)((data1 >> 4) & 0x3ff); }
	uint16_t getTextSize() { return (uint16_t)(data1 & 0xF); }

	uint16_t getFill() { return (bits2Color((uint8_t)((data2 >> 12) & 0xf))); }
	uint16_t getBorder() { return (bits2Color((uint8_t)((data2 >> 8) & 0xf))); }
	uint16_t getTextFg() { return (bits2Color((uint8_t)((data2 >> 4) & 0xf))); }
	uint16_t getTextBg() { return (bits2Color((uint8_t)(data2 & 0xf))); }

	void setLabel(char *lab) { label = lab; return; }

private:
	/* 
	data1 stores data for use in button drawing

	Bits 1-10	X position
	Bits 11-20	Y position
	Bits 11-30	Width
	Bits 31-40	Height
	Bits 41-50	X position for button label
	Bits 51-60	Y position for button label
	Bits 61-64	Font size for text
	*/
	uint64_t data1;

	/*
	data2 stores more data for use in button drawing
	These values are defined by the BIT_* definitions above

	Bits 1-4	Fill color
	Bits 5-8	Border color
	Bits 9-12	Text foreground color
	Bits 13-16	Text background color
	*/
	uint16_t data2;

	char* label;	// The label for the button. NULL if no label

};

#endif // _BUTTON_H_