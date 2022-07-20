
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
    // When the CPU reads or writes to the mapped PPU registers
    uint8_t cpuRead(uint16_t addr, bool rdonly=false);
    void cpuWrite(uint16_t addr, uint8_t data);

public:
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);

    // Clock the ppu
    void clock();

    // Reset the ppu (clears stuff)
    void reset();

    // DEBUG SCREEN STUFF
    olc::Sprite& getScreen() { return sprScreen; };
    olc::Pixel& getColorFromPaletteRam(uint8_t palette, uint8_t pixel);
    olc::Sprite& updatePaletteSprite(uint8_t i, uint8_t palette);
    PPUBus bus;

public:
    // If the current frame is complete
    bool frame_complete = false;

    // If the ppu is requesting an nmi (from VBLANK)
    bool nmi;

private:

    int16_t cycle = 0;
    int16_t scanline = 0;

    union
    {
        uint8_t bytes[0xFF];
        struct Sprite sprites[64];
    } oam;
    uint8_t oam_addr;

    bool ppu_addr_latch = false;
    uint8_t ppu_data_buffer = 0x00;
    uint16_t ppu_addr = 0x0000;

    //    uint8_t ppu_scroll_x, scroll_y;

    // PPUCTRL
    union
    {
        struct
        {
            uint8_t nametable_addr:2;
            bool increment_mode:1;
            bool spr_pattern_addr:1;
            bool bgr_pattern_addr:1;
            bool sprite_size:1;
            bool slave_mode:1; // Unused?
            bool vblank_nmi;
        };

        uint8_t regs;
    } control;

    // Some flags
    union
    {
        struct
        {
            uint8_t unused:5;
            bool overflow:1;
            bool zero_hit:1;
            bool vblank:1;
        };

        uint8_t regs;
    } status;

    // PPUMASK
    union
    {
        struct
        {
            bool grayscale:1;
            bool bgr_leftmost:1;
            bool spr_leftmost:1;
            bool render_bgr:1;
            bool render_spr:1;
            bool enhance_red:1;
            bool enhance_grn:1;
            bool enhance_blu:1;
        };

        uint8_t regs;
    } mask;

    std::shared_ptr<Cartridge> cart;

    // Colors
    olc::Pixel palScreen[0x40];

    // The screen
    olc::Sprite sprScreen = olc::Sprite(256, 240);

    // DEBUG SCREEN STUFF
    olc::Sprite sprPatternTables[2] = {olc::Sprite(128,128), olc::Sprite(128,128)};
};

