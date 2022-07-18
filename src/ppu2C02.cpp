
#include "ppu2C02.h"

ppu2C02::ppu2C02()
{
    // Clear stuff just in case
    memset(bus.patterns, 0, sizeof(bus.patterns));
    memset(bus.nametables, 0, sizeof(bus.nametables));
    memset(bus.palette, 0, sizeof(bus.palette));
    memset(oam, 0, sizeof(oam));
}

ppu2C02::~ppu2C02()
{

}

uint8_t ppu2C02::read(uint16_t addr)
{
    uint8_t data = 0x00;
    addr &= 0x3FFF; // Mirror?

    if (cart->ppuRead(addr, &data))
    {

    }

    return data;
}

void ppu2C02::write(uint16_t addr, uint8_t data)
{
    bus.write(addr, data);
}

uint8_t ppu2C02::cpuRead(uint16_t addr)
{
    addr &= 0x7; // Mirror

    if (addr==0x0) // PPUCTRL
    {

    }
    else if (addr==0x1) // PPUMASK
    {

    }
    else if (addr==0x2) // PPUSTATUS
    {

    }
    else if (addr==0x3) // OAMADDR
    {

    }
    else if (addr==0x4) // OAMDATA
    {

    }
    else if (addr==0x5) // PPUSCROLL
    {

    }
    else if (addr==0x6) // PPUADDR
    {

    }
    else if (addr==0x7) // PPUDATA
    {

    }
    return 0;
}

void ppu2C02::cpuWrite(uint16_t addr, uint8_t data)
{

}

void ppu2C02::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    bus.insertCartridge(cartridge);
}

