#include <cstring>
#include "ppubus.h"

PPUBus::PPUBus()
{

}

PPUBus::~PPUBus()
{

}

// Read from PPU's OWN BUS. Not to be confused with normal bus
// BUT if cartridge's mapper desides that the address is on the cartridge,
// then it will read from the cartridge instead of the PPU's bus
uint8_t PPUBus::read(uint16_t addr)
{
    uint8_t data = 0x00;

    // Cartridge checks if addr is to be mapped
    if (cart->ppuRead(addr, &data))
    {

    }
    // Read pattern
    else if (0x0000 <= addr && addr <= 0x1FFF)
    {
        data = patternTable[addr>0x0FFF][addr&0x3FF];
    }
    // Read nametable
    else if (0x2000 <= addr && addr <= 0x3EFF)
    {
        if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
        {
            if (0x2000 <= addr && addr <= 0x23FF)
                return nametables[0][addr&0x03FF];

            if (0x2400 <= addr && addr <= 0x27FF)
                return nametables[0][addr&0x03FF];

            if (0x2800 <= addr && addr <= 0x2BFF)
                return nametables[1][addr&0x03FF];

            if (0x2C00 <= addr && addr <= 0x2FFF)
                return nametables[1][addr&0x03FF];

        else if (cart->mirror == Cartridge::MIRROR::VERTICAL)
        {
            if (0x2000 <= addr && addr <= 0x23FF)
                return nametables[0][addr&0x03FF];

            if (0x2400 <= addr && addr <= 0x27FF)
                return nametables[1][addr&0x03FF];

            if (0x2800 <= addr && addr <= 0x2BFF)
                return nametables[0][addr&0x03FF];

            if (0x2C00 <= addr && addr <= 0x2FFF)
                return nametables[1][addr&0x03FF];

        }
    }
    // Read palette RAM
    else if (0x3F00 <= addr && addr <= 0x3F1F)
    {
        data = palette[addr-0x3F00];
    }

    return data;
}

// Write to PPU's OWN BUS, OR write somewhere in cartridge
void PPUBus::write(uint16_t addr, uint8_t data)
{
    if (cart->ppuWrite(addr, data))
    {

    }
    else if (0x0000 <= addr && addr <= 0x1FFF)
    {
        patternTable[addr>0x0FFF][addr&0x3FF] = data;
    }
    else if (0x2000 <= addr && addr <= 0x3EFF)
    {
        nametables[addr>0x23FF][(addr-0x2000)&0x1EFE] = data;
    }
    else if (0x3F00 <= addr && addr <= 0x3F1F)
    {
        palette[addr-0x3F00] = data;
    }

}

void PPUBus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    cart = cartridge;
}

