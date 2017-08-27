Hexadecimal Touch Clock ReadMe
------------------------

The Hexadecimal Clock (HexClock) Touch Version will display the current time and date in both hexadecimal and binary notation. HexClock uses a realtime clock module to keep accurate time with a battery backup in case of lost power.

Setting the clock
-----------------
To set the clock, press the display for 5 seconds. This will bring up the setup mode. For ease of use when setting the time and date, the clock uses standard decimal notation for time & date. Time is set using a 24-hour clock (i.e., hours are 0-23). When you have completed setting the proper time and exited setup mode the clock will again display time and dates in hex/decimal and binary, depending on the options set in the setup screen.

Setting the time and date: To set the hours, minutes, day, month , or year just press on the segment you want to adjust. For example, to adjust the month press on the month digits. Pressing on the upper part of the digits will increase the value. Pressing on the lower part of the digits will decrease the value. There is no option to set the seconds. When you adjust the minute setting the seconds automatically set to zero.

Adjusting colors: to adjust the foreground & background colors press on the color boxes. The screen will redraw with the new colors.

Number Base: This will switch the time/date readout on the main screen between hexadecimal and decimal number bases.

Display: The first display button will switch between 12h/24h time displays. The second display button ("Rotate") will rotate the dispplay 180 degrees.

Reset: This will reset the clock back to all the default date, time, color, and display values. To reset the clock, press the "1st", "2nd", and "3rd" buttons in that order. Any other order will not reset the clock. This is done as a safety mechanism to prevent accidental erasure.

Done: Press the "Done!" button to go back to the main clock screen.

HexClock uses American date styles (mm/dd/yy). Euro-style dates (dd/mm/yy) will have to wait for a future update. :-)


Parts
-----
These are the major parts used. Incidental parts (terminal blocks, power jacks, connecting cables, etc.) are left to the imagination of the builder.

1 - Arduino Pro Mini
1 - 7.0" 40-pin TFT Display - 800x480 (https://www.adafruit.com/products/2354)
1 - RA8875 Driver Board for 40-pin TFT Touch Displays - 800x480 (https://www.adafruit.com/products/1590)
1 - DS3231 RealTime Clock module (http://www.amazon.com/gp/product/B00LZCTMJM?psc=1&redirect=true&ref_=oh_aui_detailpage_o04_s02)
1 - 40-pin FPC Extension Board + 200mm Cable (https://www.adafruit.com/products/2098_
1 - 5V 2A power supply

Arduino/C++ Code Modules
------------------------
BitMask.h/BitMask.cpp - The Button class uses 64-bit and 16-bit numbers to store multiple options and settings. The BitMask functions help pack and unpack those bit segments.

Button.h/Button.cpp - These are the buttons used on the configuration screen. Probably the most complicated (and memory-hogging) part of the code.

ClockDigit.h/ClockDigit.cpp - Class to manage the display of digits/characters on the LED display.

ClockDisplay.h/ClockDisplay.cpp - Manages the overall display on the TFT screen, including clock digits and buttons.

EEPROMFunctions.h/EEPROMFunctions.ino - Manages Wrapper class for Arduino EEPROM functions to save and retrive long-term storage. Used to store screen calibration and clock settings between reboots.

HexClockTouch3.ino - The main HexClock code. Includes setup() and loop() routines, as well as some helper functions and global variables which probably should have gone into classes, but I got lazy. ;-)

MSTahomaBold48.c - This is the font code for the large clock digits on the main display.

RTClock.h/RTClock.cpp - Class to manage getting/setting time from the RTC module. A thin wrapper for the DS3231 libraries.

Miscellaneous Notes
-------------------
Memory: The sketch uses A LOT of memory, approximately 98% of the Pro Mini's 32K of memory. If you want to add any features you are probably going to need a bigger Arduino.

RA8875 Libraries: Adafruit has a set of libraries that manage the RA8875 driver board. The libraries worked well but drawing items on the screen, especially the large digits, was painfully slow. As a result, this program uses the Sumotoy RA8875 libraries (https://github.com/sumotoy/RA8875) which are MUCH faster. Notice that the sketch is named HexClockTouch3. Versions 1 & 2 used the Adafruit libraries.

Screen Calibration: You MUST calibrate the touch screen before it can be used. The "touch" part of the display is really just an overlay of a touch sensor onto a normal display screen. As a result, there is no direct software correlation between the two components. There is a separate sketch my GitHub library (TftCalibration.ino) which will perform this calibration. When you run this sketch it will ask you to touch several dots on the screen, then perform the math to calculate the calibration. It will then store the results of that math into the Arduino's EEPROM at a specified location. HexClockTouch will then read that EEPROM data to properly set the calibration of the screen. If you need to replace the screen you MUST reperform the calibration routine. TftCalibration.ino will also display some C code on the serial monitor if you want to program your sketch without relying on the EEPROM data.

Disclaimer
----------
I used to be a professional programmer, but that was back in the stone age when we used to chisel our code into stone tablets. I have probably violated numerous current C/C++/Arduino coding conventions in the source code. Sorry. Not sorry. The clock works, so 'nuff said ;-) If someone wants to politely point out where I can bring the code up to current standards I will be happy to do so in a future update.

Release Notes
-------------
1.0 - Initial release. Have at it. Please let me know what you think.

