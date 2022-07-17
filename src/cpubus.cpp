#include <fstream>
#include <cstring>
#include <iostream>
#include "cpubus.h"

CPUBus::CPUBus()
{
    // Clear RAM contents just in case
    for (auto &i : ram)
    {
        i = 0x00;
    }

    cpu.connectBus(this);
}

CPUBus::~CPUBus()
{

}

uint8_t CPUBus::read(uint16_t addr)
{
    if (cart->cpuRead(addr))
    {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        return ram[addr&0x07FF];
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        return ppu.cpuRead(addr&0x7);
    }

    return 0x00;
}

void CPUBus::write(uint16_t addr, uint8_t data)
{
    if (cart->cpuWrite(addr, data))
    {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        ram[addr&0x07FF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        ppu.cpuWrite(addr&0x7, data);
    }
}


