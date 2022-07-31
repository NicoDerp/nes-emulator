
#include "mapper03.h"

Mapper03::Mapper03(uint8_t p, uint8_t c) : Mapper(p, c)
{
    chrBankSelect = 0;
}

Mapper03::~Mapper03()
{

}


bool Mapper03::cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr)
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

    if (prgBanks == 1)
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

    else if (prgBanks == 2)
    {
        *mapped_addr = addr & 0x7FFF;
    }

    return true;
}

bool Mapper03::ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr)
{
    if (!(0x0000 <= addr && addr <= 0x1FFF))
        return false;

    *mapped_addr = (chrBankSelect & 0x03) * 8192 + addr;

    return true;
}

bool Mapper03::cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data)
{
    //if (!(0x8000 <= addr && addr <= 0xFFFF))
    //    return false;

    if (0x8000 <= addr && addr <= 0xFFFF)
    {
        chrBankSelect = *data & 0x03;
        *mapped_addr = addr;
    }

    // Even though mapper did some stuff it does not update the roms
    return false;
}

bool Mapper03::ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr)
{
    return false;
}
