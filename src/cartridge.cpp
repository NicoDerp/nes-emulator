#include "cartridge.h"


Cartridge::Cartridge(const std::string& filename)
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

    imageValid_ = false;

    std::ifstream file(filename, std::ios::binary);

    if (!file)
        return;

    file.read((char*)&header, 16);

    if (strncmp(header.name,"NES",3)!=0 || header.name[3]!=26)
    {
        file.close();
        return;
    }

    // NES 2.0 format not supported
    if ((header.flags7>>2)&0x3==2)
    {
        file.close();
        return;
    }

    // Trainer
    if ((header.flags6)&0x4==4)
        file.ignore(512);

    prgBanks = header.prg_rom_size;
    chrBanks = header.chr_rom_size;

    prg_rom.resize(16384*header.prg_rom_size);
    chr_rom.resize(8192*header.chr_rom_size);

    file.read((char*)prg_rom.data(), prg_rom.size());
    file.read((char*)chr_rom.data(), prg_rom.size());

    mapperID = (header.flags7&0xF0)|(header.flags6>>4);

    file.close();
    imageValid_ = true;
}

Cartridge::~Cartridge()
{

}

bool Cartridge::cpuRead(uint16_t &addr)
{
    return false;
}

bool Cartridge::ppuRead(uint16_t &addr)
{
    return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
    return false;
}


