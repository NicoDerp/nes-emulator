
#include "cartridge.h"
#include "helper.h"


Cartridge::Cartridge(const std::string& filename)
{
    struct iNesHeader
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

    std::ifstream file(filename, std::ifstream::binary);

    if (!file)
        return;

    file.read((char*)&header, sizeof(iNesHeader));

    if (strncmp(header.name,"NES",3)!=0 || header.name[3]!=26)
    {
        file.close();
        return;
    }

    // NES 2.0 format not supported
    if ((header.flags7>>2)&0x3==2)
    {
        printf("[ERROR] Nes file format not supported");
        file.close();
        return;
    }

    // Trainer
    if (header.flags6&0x4)
    {
        printf("[NOTE] The nes rom has a trainer");
        file.ignore(512);
    }

    prgBanks = header.prg_rom_size;
    chrBanks = header.chr_rom_size;

    prg_rom.resize(16384*prgBanks);
    chr_rom.resize(8192*chrBanks);

    file.read((char*)prg_rom.data(), prg_rom.size());
    file.read((char*)chr_rom.data(), prg_rom.size());
    file.close();

    mapperID = (header.flags7&0xF0)|(header.flags6>>4);
    mirror = (header.flags6&0x1) ? VERTICAL : HORIZONTAL;

    if (mapperID == 0x00)
        mapper = std::unique_ptr<Mapper>(new Mapper00(prgBanks, chrBanks));
    else
    {
        printf("[ERROR] Unknown mapper 0x%s. Probably coming soon...", hex(mapperID,2).c_str());
        return;
    }

    printf("PRG BANKS: %d\n", prgBanks);
    printf("CHR BANKS: %d\n", chrBanks);
    printf("MAPPER ID: %d\n", mapperID);

    imageValid_ = true;
}

Cartridge::~Cartridge()
{

}

bool Cartridge::imageValid()
{
    return imageValid_;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t* data)
{
    uint32_t mapped_addr = 0;

    if (!mapper->cpuMapReadAddr(addr, &mapped_addr))
        return false;

    *data = prg_rom[mapped_addr];

    return true;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t* data)
{
    if (chrBanks == 0)
        return false;

    uint32_t mapped_addr = 0;

    if (!mapper->ppuMapReadAddr(addr, &mapped_addr))
        return false;

    *data = chr_rom[mapped_addr];

    return true;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;

    if (!mapper->cpuMapWriteAddr(addr, &mapped_addr))
        return false;

    prg_rom[mapped_addr] = data;

    return true;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
    if (chrBanks == 0)
        return false;

    uint32_t mapped_addr = 0;

    if (!mapper->ppuMapWriteAddr(addr, &mapped_addr))
        return false;

    chr_rom[mapped_addr] = data;

    return true;
}


