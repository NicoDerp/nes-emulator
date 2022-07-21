#include <fstream>
#include <cstring>
#include <iostream>
#include "cpubus.h"
#include "helper.h"

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

uint8_t CPUBus::read(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;
    if (cart->cpuRead(addr, &data))
    {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        data = ram[addr&0x07FF];
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        data = ppu.cpuRead(addr, rdonly);
    }

    return data;
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
        ppu.cpuWrite(addr, data);
    }
}

void CPUBus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    cart = cartridge;
    ppu.insertCartridge(cartridge);
}

void CPUBus::reset()
{
    cpu.reset();
    ppu.reset();
    systemClockCount = 0;
}

void CPUBus::clock()
{
    ppu.clock();

    if (++systemClockCount == 3)
    {
        cpu.clock();
        systemClockCount = 0;
    }

    if (ppu.nmi)
    {
        ppu.nmi = false;
        cpu.nmi();
    }
}

