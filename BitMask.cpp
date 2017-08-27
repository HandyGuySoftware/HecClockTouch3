// Data blob update functions
// 
// 

#include "BitMask.h"

void updateSegment(uint64_t *data, uint16_t input, uint64_t eraseMask, uint64_t addMask, uint8_t bitShift)
{
	*(data) &= eraseMask;
	*(data) |= (((uint64_t)input & addMask) << bitShift);
}

void updateSegment(uint16_t *data, uint16_t input, uint16_t eraseMask, uint8_t addMask, uint8_t bitShift)
{
	*(data) &= eraseMask;
	*(data) |= (((uint16_t)input & addMask) << bitShift);
}

