
#pragma once

#include "mapper.h"
#include <vector>

class Mapper01 : public Mapper
{
public:
    Mapper01(uint8_t p, uint8_t c);
    ~Mapper01();

public:
    bool cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data) override;
    bool ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr) override;

    bool cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data) override;
    bool ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr) override;

    void reset() override;
    MIRROR mirror() override;

private:
    uint8_t prgBankSelect;

    uint8_t chrBankOneSelect;
    uint8_t chrBankTwoSelect;

    uint8_t load_register;
    uint8_t times_shifted;

    union
    {
        struct
        {
            uint8_t mir:2;
            uint8_t prgBankMode:2;
            uint8_t chrBankMode:1;
            uint8_t unused:3;
        };

        uint8_t reg;
    } control;

    std::vector<uint8_t> ram;

    MIRROR mirrormode = MIRROR::HORIZONTAL;
};

