#include <cstring>
#include "ppubus.h"

PPUBus::PPUBus(ppu2C02* n)
{
    this->ppu = n;

    // Clear stuff just in case
    memset(patterns, 0, sizeof(patterns));
    memset(nametables, 0, sizeof(nametables));
    for (auto &i : palette)
    {
        i = 0x00;
    }
    /*
    for (auto &i : ppu->omc)
    {
        i = 0x00;
    }
    */

}

PPUBus::~PPUBus()
{

}

uint8_t PPUBus::read(uint16_t addr)
{
    // Read pattern
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        return ((uint8_t*)&patterns)[addr];
        //        uint8_t offset = addr & 0x8;
        //        Tile t = patterns[addr-offset];
        //        if (offset == 0)
        //            return t.bits0;
        //        else
        //            return t.bits1;
    }
    // Read nametable
    else if (addr >= 0x2000 && addr <= 0x2FFF)
    {
        return ((uint8_t*)&nametables)[addr-0x2000];
    }

    return 0x00;
}

void PPUBus::write(uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        //ram[addr&0x07FF] = data;
    }
}

