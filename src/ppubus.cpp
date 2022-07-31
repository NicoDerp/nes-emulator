#include <cstring>
#include "ppubus.h"
#include "helper.h"

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
        uint8_t i = ((addr-0x2000)/0x0400)%3;
        if (cart->mirror() == MIRROR::HORIZONTAL)
        {
            // Mirrors
            if (i == 0)
                data = nametables[0][addr&0x03FF];

            if (i == 1)
                data = nametables[0][addr&0x03FF];

            if (i == 2)
                data = nametables[1][addr&0x03FF];

            if (i == 3)
                data = nametables[1][addr&0x03FF];
        }
        else if (cart->mirror() == MIRROR::VERTICAL)
        {
            if (i == 0)
                data = nametables[0][addr&0x03FF];

            if (i == 1)
                data = nametables[1][addr&0x03FF];

            if (i == 2)
                data = nametables[0][addr&0x03FF];

            if (i == 3)
                data = nametables[1][addr&0x03FF];
        }
    }
    // Read palette RAM
    else if (0x3F00 <= addr && addr <= 0x3FFF)
    {
        data = palette[addr&0x1F];
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
        // 0x03FF
        patternTable[addr>0x0FFF][addr&0x0FFF] = data;
    }
    else if (0x2000 <= addr && addr <= 0x3EFF)
    {
        // For correct mirroring
        uint8_t i = ((addr-0x2000)/0x0400)%3;
        if (cart->mirror() == MIRROR::HORIZONTAL)
        {
            // Mirrors
            if (i == 0)
                nametables[0][addr&0x03FF] = data;

            if (i == 1)
                nametables[0][addr&0x03FF] = data;

            if (i == 2)
                nametables[1][addr&0x03FF] = data;

            if (i == 3)
                nametables[1][addr&0x03FF] = data;
        }
        else if (cart->mirror() == MIRROR::VERTICAL)
        {
            if (i == 0)
                nametables[0][addr&0x03FF] = data;

            if (i == 1)
                nametables[1][addr&0x03FF] = data;

            if (i == 2)
                nametables[0][addr&0x03FF] = data;

            if (i == 3)
                nametables[1][addr&0x03FF] = data;
        }
    }
    else if (0x3F00 <= addr && addr <= 0x3FFF)
    {
        // Not mine just testing
        //addr &= 0x1F;
        //if (addr == 0x0010) addr = 0x0000;
		//if (addr == 0x0014) addr = 0x0004;
		//if (addr == 0x0018) addr = 0x0008;
		//if (addr == 0x001C) addr = 0x000C;
        //palette[addr] = data;
        palette[addr&0x1F] = data;
    }

}

void PPUBus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    cart = cartridge;
}

