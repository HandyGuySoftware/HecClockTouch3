// Functions to read and write datato the Arduino EEPROM memory
// 
// 

#include "EEPROMFunctions.h"

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, uint32_t value)
{
	//Decomposition from a long to 4 bytes by using bitshift.
	//One = Most significant -> Four = Least significant byte

	byte four = (value & 0xFF);
	byte three = ((value >> 8) & 0xFF);
	byte two = ((value >> 16) & 0xFF);
	byte one = ((value >> 24) & 0xFF);

	// Write the 4 bytes into the eeprom memory. Update function only writes if value was updated (Minimizd EEPROM wear)
	EEPROM.update(address, one);
	EEPROM.update(address + 1, two);
	EEPROM.update(address + 2, three);
	EEPROM.update(address + 3, four);
}

//This function will return an unsigned 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
uint32_t EEPROMReadUnsignedLong(int address)
{
	//Read the 4 bytes from the eeprom memory.
	uint32_t one = (uint32_t)EEPROM.read(address);
	uint32_t two = (uint32_t)EEPROM.read(address + 1);
	uint32_t three = (uint32_t)EEPROM.read(address + 2);
	uint32_t four = (uint32_t)EEPROM.read(address + 3);
		
	//Return the recomposed long by using bitshift.
	return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

//This function will return a signed 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
int32_t EEPROMReadSignedLong(int address)
{
	//Read the 4 bytes from the eeprom memory.
	int32_t one = (uint32_t)EEPROM.read(address);
	int32_t two = (uint32_t)EEPROM.read(address + 1);
	int32_t three = (uint32_t)EEPROM.read(address + 2);
	int32_t four = (uint32_t)EEPROM.read(address + 3);

	//Return the recomposed long by using bitshift.
	return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

