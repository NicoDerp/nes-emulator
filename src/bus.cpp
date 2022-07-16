#include <fstream>
#include <cstring>
#include <iostream>
#include "bus.h"

Bus::Bus()
{
    // Clear RAM contents just in case
    for (auto &i : ram)
    {
        i = 0x00;
    }

    cpu.connectBus(this);
}

Bus::~Bus()
{

}

uint8_t Bus::read(uint16_t addr)
{
    if (addr >= 0x0000 && addr <= 0xFFFF)
        return ram[addr];

    return 0x00;
}

void Bus::write(uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= 0xFFFF)
        ram[addr] = data;
}

bool Bus::load(std::string filename)
{
    std::string line;
    std::ifstream file(filename, std::ios::binary);

    if (!file)
        return false;

    char magic[4];
    file.read((char*)magic, 4);

    if (strncmp(magic,"NES",3)==0 && magic[3]==26)
    {
        file.read((char*)&rom.prg_rom_size, 1);
        file.read((char*)&rom.chr_rom_size, 1);
        file.read((char*)&rom.flags6, 1);
        file.read((char*)&rom.flags7, 1);

        printf("prg_rom_size: %d\n", rom.prg_rom_size);
        printf("chr_rom_size: %d\n", rom.chr_rom_size);

        if (rom.flags7.nes_two)
        {
            file.close();
            return false;
        }

        // Skip to byte 9
        file.seekg(9);
        file.read((char*)&rom.flags9, 1);

        // Skip the garbage
        file.seekg(16);

        // Trainer
        if ((bool)rom.flags6.trainer)
        {
            rom.trainer = (char*)malloc(512);
            file.read((char*)rom.trainer, 512);
        }

        rom.prg_rom = (char*)malloc(16384*rom.prg_rom_size);
        rom.chr_rom = (char*)malloc(8192*rom.chr_rom_size);

        file.read((char*)rom.prg_rom, 16384*rom.prg_rom_size);
        file.read((char*)rom.chr_rom, 8192*rom.chr_rom_size);

        for (uint16_t offset=0x0000;offset<16384*rom.prg_rom_size;offset++)
        {
            ram[0x8000+offset] = rom.prg_rom[offset];
        }

        file.close();
        return true;
    }
    else
    {
        file.close();
        return false;
    }
}

