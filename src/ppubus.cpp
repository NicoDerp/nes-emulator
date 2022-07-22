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
        if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
        {
            if (0x2000 <= addr && addr <= 0x23FF)
                data = nametables[0][addr&0x03FF];

            if (0x2400 <= addr && addr <= 0x27FF)
                data = nametables[0][addr&0x03FF];

            if (0x2800 <= addr && addr <= 0x2BFF)
                data = nametables[1][addr&0x03FF];

            if (0x2C00 <= addr && addr <= 0x2FFF)
                data = nametables[1][addr&0x03FF];
        }
        else if (cart->mirror == Cartridge::MIRROR::VERTICAL)
        {
            if (0x2000 <= addr && addr <= 0x23FF)
                data = nametables[0][addr&0x03FF];

            if (0x2400 <= addr && addr <= 0x27FF)
                data = nametables[1][addr&0x03FF];

            if (0x2800 <= addr && addr <= 0x2BFF)
                data = nametables[0][addr&0x03FF];

            if (0x2C00 <= addr && addr <= 0x2FFF)
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
        if (cart->mirror == Cartridge::MIRROR::HORIZONTAL)
        {
            if (0x2000 <= addr && addr <= 0x23FF)
                nametables[0][addr&0x03FF] = data;

            if (0x2400 <= addr && addr <= 0x27FF)
                nametables[0][addr&0x03FF] = data;

            if (0x2800 <= addr && addr <= 0x2BFF)
                nametables[1][addr&0x03FF] = data;

            if (0x2C00 <= addr && addr <= 0x2FFF)
                nametables[1][addr&0x03FF] = data;
        }
        else if (cart->mirror == Cartridge::MIRROR::VERTICAL)
        {
            if (0x2000 <= addr && addr <= 0x23FF)
                nametables[0][addr&0x03FF] = data;

            if (0x2400 <= addr && addr <= 0x27FF)
                nametables[1][addr&0x03FF] = data;

            if (0x2800 <= addr && addr <= 0x2BFF)
                nametables[0][addr&0x03FF] = data;

            if (0x2C00 <= addr && addr <= 0x2FFF)
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
        palette[addr&0x1F] = data;
    }

}

void PPUBus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    cart = cartridge;
}

