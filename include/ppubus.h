#pragma once

#include <memory>
#include "cartridge.h"

class PPUBus
{
public:
	PPUBus();
	~PPUBus();

// Devices
public:
    // PPU
    //ppu2C02* ppu;

    // Patterns
    uint8_t patternTable[2][4096];

    // Nametables
    uint8_t nametables[2][1024];

    // Palette
    uint8_t palette[32];

    // Cartridge
    std::shared_ptr<Cartridge> cart;

// Bus operations
public:
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

public:
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
};
