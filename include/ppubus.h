#pragma once

#include <array>
#include <memory>
#include "cpu6502.h"
#include "ppu2C02.h"
#include "cartridge.h"

struct Tile
{
    uint8_t bits0;
    uint8_t bits1;
};

struct Nametable
{
    uint8_t cells[960];
    uint8_t attrs[64];
};

class PPUBus
{
public:
	PPUBus(ppu2C02* n);
	~PPUBus();

// Devices
public:
	// CPU bus
    CPUBus* cpuBus;

    // PPU
    ppu2C02* ppu;

    // Patterns
    struct Tile patterns[512];

    // Nametables
    struct Nametable nametables[2];

    // Palette
    uint8_t palette[32];

    // Cartridge
    std::shared_ptr<Cartridge> cart;

// Functions
public:
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
};
