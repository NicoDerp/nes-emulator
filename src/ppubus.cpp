#include <cstring>
#include "ppubus.h"

PPUBus::PPUBus()
{

}

PPUBus::~PPUBus()
{

}

// Shouldn't have two reads for ppu and ppubus so need to figure stuff out
uint8_t PPUBus::read(uint16_t addr)
{
    // Read pattern
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        return ((uint8_t*)&patterns)[addr];
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

