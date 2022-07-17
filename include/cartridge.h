
#pragma once

#include <vector>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

class Cartridge
{
public:
    Cartridge(const std::string& filename);
    ~Cartridge();

private:
    std::vector<uint8_t> prg_rom;
    std::vector<uint8_t> chr_rom;

    uint8_t mapperID;
    uint8_t prgBanks;
    uint8_t chrBanks;

public:
    uint8_t cpuRead(uint16_t addr);
    bool cpuWrite(uint16_t addr, uint8_t data);

    uint8_t ppuRead(uint16_t addr);
    bool ppuWrite(uint16_t addr, uint8_t data);
};
