
#pragma once

#include "mapper.h"

class Mapper01 : public Mapper
{
public:
    Mapper01(uint8_t p, uint8_t c);
    ~Mapper01();

public:
    bool cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) override;
    bool ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) override;

    bool cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr) override;
    bool ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr) override;

};

