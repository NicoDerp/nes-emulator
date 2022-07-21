//#ifndef INCLUDE_CPUBUS_H
//#define INCLUDE_CPUBUS_H
#pragma once

#include <array>
#include <memory>
#include "cpu6502.h"
#include "ppu2C02.h"
#include "cartridge.h"

class CPUBus
{
public:
	CPUBus();
	~CPUBus();

// Devices
public:
	// CPU
    cpu6502 cpu;

    // PPU
    ppu2C02 ppu;

    // RAM
	std::array<uint8_t, 2048> ram;

    // Cartridge
    std::shared_ptr<Cartridge> cart;

    // APU
    // Controls
    // Mapper
    // Stuff

// Functions
public:
	uint8_t read(uint16_t addr, bool rdonly=false);
	void write(uint16_t addr, uint8_t data);

public:
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();

private:
    uint8_t systemClockCount = 0;
};

//#endif /* INCLUDE_CPU_BUS_H */
