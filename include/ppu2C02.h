
#pragma once

#include "olcPixelGameEngine.h"
#include "ppubus.h"

// Forward declarations
class Bus;
class Cartridge;

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

public:
    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t data);

public:
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
    olc::Sprite& getScreen() { return sprScreen; };

public:
    bool frame_complete = false;

private:
    PPUBus bus;

    int16_t cycle = 0;
    int16_t scanline = 0;

    struct Sprite oam[64];

    std::shared_ptr<Cartridge> cart;

    olc::Pixel palScreen[0x40];
    olc::Sprite sprScreen = olc::Sprite(256, 240);

private:
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

};

