
#pragma once

#include "mapper.h"

class Mapper03 : public Mapper
{
public:
    Mapper03(uint8_t p, uint8_t c);
    ~Mapper03();

public:
    bool cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) override;
    bool ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) override;

    bool cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data) override;
    bool ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr) override;

private:
    uint8_t chrBankSelect;

};

