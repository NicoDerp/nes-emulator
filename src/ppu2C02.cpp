
#include "ppu2C02.h"

ppu2C02::ppu2C02()
{
    bus = new PPUBus();
}

ppu2C02::~ppu2C02()
{
    delete bus;
}

uint8_t ppu2C02::read(uint16_t addr)
{
    return bus.read(addr);
}

void ppu2C02::write(uint16_t addr, uint8_t data)
{
    bus.write(addr, data);
}

uint8_t ppu2C02::cpuRead(uint16_t addr)
{

}

void ppu2C02::cpuWrite(uint16_t addr, uint8_t data)
{

}


