#ifndef INCLUDE_BUS_H
#define INCLUDE_BUS_H

#include <array>
#include "cpu6502.h"

class Bus
{
public:
	Bus();
	~Bus();

// Devices
public:
	// CPU
    cpu6502 cpu;

	// Fake RAM
	std::array<uint8_t, 64 * 1024> ram;

// Functions
public:
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
};

#endif /* INCLUDE_BUS_H */
