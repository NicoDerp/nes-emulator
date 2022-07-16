//#ifndef INCLUDE_BUS_H
//#define INCLUDE_BUS_H
#pragma once

#include <array>
#include "cpu6502.h"

struct flags6
{
    bool mirror:1;
    bool prg_ram:1;
    bool trainer:1;
    bool ign_mir:1;
    char lw_map:4;
};

struct flags7
{
    bool unisystem:1;
    bool playchoice:1;
    char nes_two:2;
    char up_map:4;
};

struct flags9
{
    bool tvsystem:1;
    char unused:7;
};

struct Rom
{
    char* prg_rom;
    char* chr_rom;

    char* trainer;

    uint8_t prg_rom_size;
    uint8_t chr_rom_size;

    struct flags6 flags6;
    struct flags7 flags7;
    struct flags9 flags9;
};

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

    Rom rom;

// Functions
public:
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

    bool load(std::string filename);
};

//#endif /* INCLUDE_BUS_H */
