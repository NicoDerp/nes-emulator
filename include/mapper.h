
#pragma once

#include <cstdint>

class Mapper
{

public:
    Mapper(uint8_t p, uint8_t c);
    ~Mapper();

public:
    virtual bool cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) = 0;
    virtual bool ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) = 0;

    virtual bool cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr) = 0;
    virtual bool ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr) = 0;


protected:
    uint8_t prg_banks = 0x00;
    uint8_t chr_banks = 0x00;
};

