
#pragma once

#include <vector>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

#include "mapper00.h"

enum MIRROR
{
    HORIZONTAL,
    VERTICAL
};

class Cartridge
{
public:
    Cartridge(const std::string& filename);
    ~Cartridge();

private:
    std::vector<uint8_t> prg_rom;
    std::vector<uint8_t> chr_rom;

    enum MIRROR mirror;
    uint8_t mapperID;
    uint8_t prgBanks;
    uint8_t chrBanks;

    bool imageValid_ = false;

    Mapper* mapper;

public:
    bool imageValid();

    bool cpuRead(uint16_t addr, uint8_t* data);
    bool ppuRead(uint16_t addr, uint8_t* data);

    bool cpuWrite(uint16_t addr, uint8_t data);
    bool ppuWrite(uint16_t addr, uint8_t data);
};

