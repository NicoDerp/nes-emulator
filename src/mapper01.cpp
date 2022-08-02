
#include "mapper01.h"

// (https://www.nesdev.org/wiki/MMC1)

Mapper01::Mapper01(uint8_t p, uint8_t c) : Mapper(p, c)
{
    ram.resize(32 * 1024);
}

Mapper01::~Mapper01()
{

}


bool Mapper01::cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data)
{
    // 8 KB RAM bank (optional)
    if (0x6000 <= addr && addr <= 0x7FFF)
    {
        *mapped_addr = 0xFFFFFFFF;
        *data = ram[addr & 0x1FFF];
        return true;
    }

    if (!(0x8000 <= addr && addr <= 0xFFFF))
        return false;

    // Single 32 KB at 0x8000
    if (control.prgBankMode <= 1)
    {
        // i * 16 KB + offset
        *mapped_addr = prgBankSelect * 0x8000 + (addr & 0x7FFF);
    }

    // Fix first bank at 0x8000 and switch 16 KB bank at 0xC000
    if (control.prgBankMode == 2)
    {
        if (addr <= 0xBFFF)
        {
            *mapped_addr = (prgBanks - 1) * 0x4000 + (addr & 0x3FFF);
        }

        if (addr >= 0xC000)
        {
            *mapped_addr = prgBankSelect * 0x4000 + (addr & 0x3FFF);
        }
    }

    // Fix last bank at 0xC000 and switch 16 KB bank at 0x8000
    if (control.prgBankMode == 3)
    {
        if (addr <= 0xBFFF)
        {
            *mapped_addr = prgBankSelect * 0x4000 + (addr & 0x3FFF);
        }

        if (addr >= 0xC000)
        {
            *mapped_addr = (prgBanks - 1) * 0x4000 + (addr & 0x3FFF);
        }
    }

    return true;
}

bool Mapper01::ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr)
{
    if (!(0x000 <= addr && addr <= 0x1FFF))
        return false;

    if (chrBanks == 0)
    {
        // Ram
        *mapped_addr = addr;
    }
    else
    {
        // Switch 8 KB at a time
        if (control.chrBankMode == 0)
        {
            *mapped_addr = chrBankOneSelect * 0x2000 + (addr & 0x1FFF);
        }

        // Switch two seperate 4 KB banks
        if (control.chrBankMode == 1)
        {
            if (0x0000 <= addr && addr <= 0x0FFF)
            {
                *mapped_addr = chrBankOneSelect * 0x1000 + (addr & 0x0FFF);
            }

            if (0x1000 <= addr && addr <= 0x1FFF)
            {
                *mapped_addr = chrBankTwoSelect * 0x1000 + (addr & 0x0FFF);
            }
        }
    }

    return true;
}

bool Mapper01::cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr, uint8_t* data)
{
    if (0x6000 <= addr && addr <= 0x7FFF)
    {
        *mapped_addr = 0xFFFFFFFF;
        ram[addr & 0x1FFF] = *data;
        return true;
    }

    if (0x8000 <= addr && addr <= 0xFFFF)
    {
        if (*data & 0x80)
        {
            // Bit 7 is set so reset
            load_register = 0x00;
            times_shifted = 0x00;
            control.reg |= 0x0C;
        }
        else if (++times_shifted == 5)
        {
            // Update one of the registers
            uint8_t zone = addr >> 12;

            if (zone == 0x08)
                control.reg = load_register;

            if (zone == 0x0A)
                chrBankOneSelect = load_register;

            if (zone == 0x0C)
                chrBankTwoSelect = load_register;

            if (zone == 0x0E)
                prgBankSelect = load_register;

            load_register = 0x00;
        }
        else
        {
            load_register |= (*data & 0x01) << 4;
            load_register >>= 1;
        }

    }

    // We do not update roms
    return false;
}

bool Mapper01::ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr)
{
    if (!(0x0000 <= addr && addr <= 0x1FFF))
        return false;

    if (chrBanks == 0)
    {
        *mapped_addr = addr;
        return true;
    }

    return false;
}

void Mapper01::reset()
{
    prgBankSelect = 0x00;

    chrBankOneSelect = 0x00;
    chrBankTwoSelect = 0x00;

    load_register = 0x00;
    times_shifted = 0x00;

    control.chrBankMode = 1;
    control.prgBankMode = 3;
    control.mir = 0;
    // control.reg = 0x1C
}

MIRROR Mapper01::mirror()
{
    return mirrormode;
}

