// BitMask.h

#ifndef _BITMASK_h
#define _BITMASK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Update a data blob with discrete segments
// See definitions of Class Button for definitions of the data segments 

// Update a 64-bit data blob
void updateSegment(uint64_t *data, uint16_t input, uint64_t eraseMask, uint64_t addMask, uint8_t bitShift);

// Update a 16-bit data blob
void updateSegment(uint16_t *data, uint16_t input, uint16_t eraseMask, uint8_t addMask, uint8_t bitShift);

#endif // _BITMASK_h

