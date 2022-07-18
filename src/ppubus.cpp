#include <cstring>
#include "ppubus.h"

PPUBus::PPUBus()
{

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

void PPUBus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    cart = cartridge;
}

