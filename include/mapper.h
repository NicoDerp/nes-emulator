
#pragma once

#include <cstdint>

enum MIRROR
{
    HARDWARE,
    HORIZONTAL,
    VERTICAL
};


class Mapper
{

public:
    Mapper(uint8_t p, uint8_t c);
    ~Mapper();

public:
    virtual bool cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data) = 0;
    virtual bool ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) = 0;

    virtual bool cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data) = 0;
    virtual bool ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr) = 0;

    virtual void reset() = 0;
    virtual MIRROR mirror() { return MIRROR::HARDWARE; };

protected:
    uint8_t prgBanks = 0x00;
    uint8_t chrBanks = 0x00;
};

