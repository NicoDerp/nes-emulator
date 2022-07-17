
#pragma once

// Forward declarations
class Bus;
class PPUBus;

struct Sprite
{
    uint8_t y;
    uint8_t tile;
    uint8_t attr;
    uint8_t x;
};

class ppu2C02
{
public:
    ppu2C02();
    ~ppu2C02();

    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t data);

private:
    PPUBus* bus = nullptr;
    struct Sprite oam[64];

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

};

