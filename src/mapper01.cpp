
#include "mapper01.h"

Mapper01::Mapper01(uint8_t p, uint8_t c) : Mapper(p, c)
{

}

Mapper01::~Mapper01()
{

}


bool Mapper01::cpuMapReadAddr(uint16_t addr, uint32_t* mapped_addr)
{
    return false;
}

bool Mapper01::ppuMapReadAddr(uint16_t addr, uint32_t* mapped_addr)
{
    return false;
}

bool Mapper01::cpuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr)
{
    return false;
}

bool Mapper01::ppuMapWriteAddr(uint16_t addr, uint32_t* mapped_addr)
{
    return false;
}

