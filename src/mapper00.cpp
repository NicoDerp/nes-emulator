
#include "mapper00.h"
//#include <iostream>
//#include "helper.h"


Mapper00::Mapper00(uint8_t p, uint8_t c) : Mapper(p, c)
{

}

Mapper00::~Mapper00()
{

}


bool Mapper00::cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr)
{
    if (!(0x8000 <= addr && addr <= 0xFFFF))
        return false;

    /** 16KB ROM
     *
     * One 16KB bank
     *
     *  CPU ADDRESS BUS     PRG ROM
     * 0x8000 - 0xBFFF -> 0x0000 - 0x3FFF
     * 0xC000 - 0xFFFF -> 0x0000 - 0x3FFF
     *
    ***/

    if (prg_banks == 1)
    {
        *mapped_addr = addr & 0x3FFF; // addr % 16kb
    }

    /** 32KB PRG
     *
     * Two 16KB banks
     *
     *  CPU ADDRESS BUS     PRG ROM
     * 0x8000 - 0xFFFF -> 0x0000 - 0x7FFF
     *
    ***/

    else if (prg_banks == 2)
    {
        *mapped_addr = addr & 0x7FFF;
    }

    //    printf("BANKS: %d\nADDR: 0x%s\nMAPD: 0x%s\n\n", prg_banks, hex(addr,4).c_str(), hex(*mapped_addr,8).c_str());

    return true;
}

bool Mapper00::ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr)
{
    if (!(0x0000 <= addr && addr <= 0x1FFF))
        return false;

    // No mapping required
    // 0x0000 - 0x1FFF -> 0x0000 - 0x1FFF

    *mapped_addr = addr;

    return true;
}

bool Mapper00::cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr)
{
    if (!(0x8000 <= addr && addr <= 0xFFFF))
        return false;

    // The same as cpuMapReadAddr
    if (prg_banks == 1)
    {
        *mapped_addr = addr & 0x3FFF;
    }
    else if (prg_banks == 2)
    {
        *mapped_addr = addr & 0x7FFF;
    }

    return true;
}

bool Mapper00::ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr)
{
    if (!(0x0000 <= addr && addr <= 0x1FFF))
        return false;

    *mapped_addr = addr;

    return true;
}

