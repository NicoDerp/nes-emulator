#include "cartridge.h"


Cartridge::Cartridge()
{
    struct
    {
        char name[4];
        uint8_t prg_rom_size;
        uint8_t chr_rom_size;
        uint8_t flags6;
        uint8_t flags7;
        uint8_t flags8;
        uint8_t flags9;
        uint8_t flags10;
        char unused[5];
    } header;

    std::ifstream file(filename, std::ios::binary);

    if (!file)
        return;

    file.read(&header, 16);

    if (strncmp(header.name,"NES",3)!=0 || header.name[3]!=26)
    {
        file.close();
        return false;
    }

    // NES 2.0 format not supported
    if ((flags7>>2)&0x3==2)
    {
        file.close();
        return false;
    }

    // Trainer
    if (flags6.trainer)
        file.ignore(512);

    prgBanks = prg_rom_size;
    chrBanks = chr_rom_size;

    prg_rom.resize(16384*prg_rom_size);
    chr_rom.resize(8192*chr_rom_size);

    file.read((char*)prg_rom.data(), prg_rom.size());
    file.read((char*)chr_rom.data(), prg_rom.size());

    mapperID = (header.flags7&0xF0)|(header.flags6>>4);

        //for (uint16_t offset=0x0000;offset<16384*prg_rom_size;offset++)
        //{
        //    ram[0x8000+offset] = prg_rom[offset];
        //}

    file.close();
}

Cartridge::~Cartridge()
{

}

uint8_t Cartridge::cpuRead(uint16_t addr)
{

}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
    return false;
}

uint8_t Cartridge::ppuRead(uint16_t addr)
{

}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
    return false;
}


