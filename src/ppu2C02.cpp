
#include "ppu2C02.h"
#include "helper.h"


ppu2C02::ppu2C02()
{
    // Clear stuff just in case
    memset((uint8_t*)bus.patternTable, 0, sizeof(sizeof(bus.patternTable)));
    memset((uint8_t*)bus.nametables, 0, sizeof(bus.nametables));
    memset(bus.palette, 0, sizeof(bus.palette));
    memset(oam.bytes, 0, sizeof(oam));

    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);

}

ppu2C02::~ppu2C02()
{

}

uint8_t ppu2C02::cpuRead(uint16_t addr, bool rdonly)
{
    addr &= 0x7; // Mirror

    if (addr==0x0) // PPUCTRL
    {
        // Write-only
    }
    else if (addr==0x1) // PPUMASK
    {
        // Write-only
    }
    else if (addr==0x2) // PPUSTATUS
    {
        if (rdonly)
            return 0x00;

        // Emulate ppu-timing, probably not of great importance. Couldn't figure it out anyways
        status.vblank = false;
        ppu_addr_latch = false;

        status.vblank = true; // TODO: REMOVE

        uint8_t tmp = (status.regs&0xE0) | (ppu_data_buffer&0x1F);
        status.vblank = false;

        // Top three bits of status + some ppu noise?
        return tmp;
    }
    else if (addr==0x3) // OAMADDR
    {
        // Write-only
    }
    else if (addr==0x4) // OAMDATA
    {
        return oam.bytes[oam_addr++];
    }
    else if (addr==0x5) // PPUSCROLL
    {
        // Write-only
    }
    else if (addr==0x6) // PPUADDR
    {
        // Write-only
    }
    else if (addr==0x7) // PPUDATA
    {
        if (rdonly)
            return 0x00;

        // Screen is turned off or VBLANK
        //        if (status.vblank || (!mask.render_spr && !mask.render_bgr))

        uint8_t data = ppu_data_buffer;
        ppu_data_buffer = bus.read(ppu_addr);

        // PPUDATA read is delayed except for reading palettes
        if (ppu_addr>0x3F00)
            data = ppu_data_buffer;

        ppu_addr += (control.increment_mode ? 1 : 32); // im*31+1

        return data;
    }
    else
    {
        // Can't get here, but I don't trust myself enough :o
        printf("[WARNING] Unreachable location in ppu2C02::cpuRead reached %d. Bug in mirroring\n",addr);
    }
    return 0x00;
}

void ppu2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x7; // Mirror

    if (addr==0x0) // PPUCTRL
    {
        control.regs = data;
    }
    else if (addr==0x1) // PPUMASK
    {
        mask.regs = data;
    }
    else if (addr==0x2) // PPUSTATUS
    {
        // Read-only
    }
    else if (addr==0x3) // OAMADDR
    {
        oam_addr = data;
    }
    else if (addr==0x4) // OAMDATA
    {
        oam.bytes[oam_addr] = data;
        oam_addr++;
    }
    else if (addr==0x5) // PPUSCROLL
    {
        // TODO
    }
    else if (addr==0x6) // PPUADDR
    {
        if (ppu_addr_latch)
            ppu_addr |= data;
        else
            ppu_addr = data<<8;

        //if (ppu_addr_latch)
        //    printf("PPUADDR: 0x%s\n",hex(ppu_addr,4).c_str());

        ppu_addr_latch = !ppu_addr_latch;
    }
    else if (addr==0x7) // PPUDATA
    {
        bus.write(ppu_addr, data);
        ppu_addr += (control.increment_mode ? 32 : 1); // im*31+1
    }
    else
    {
        printf("[WARNING] Unreachable location in ppu2C02::cpuWrite reached %d. Bug in mirroring\n",addr);
    }
}

void ppu2C02::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    cart = cartridge;
    bus.insertCartridge(cartridge);
}

void ppu2C02::clock()
{
    //sprScreen.SetPixel(cycle-1, scanline, palScreen[(rand()%2) ? 0x3F : 0x30]);
    //    sprScreen.SetPixel(cycle-1, scanline, palScreen[getColorFromPaletteRam()]);
    // x

    //printf("NMI ON FRAME COMPLETE: %d\n", control.vblank_nmi);

    // VBLANK!
    if (scanline >= 241 && scanline < 261)
    {
        // END OF FRAME!!
        if (scanline == 241 && cycle == 1)
        {
            status.vblank = true;

            if (control.vblank_nmi)
                nmi = true;
        }

    }

    cycle++;
    if (cycle >= 341)
    {
        // y
        cycle = 0;
        scanline++;

        // VBLANK
        if (scanline >= 261)
        {
            scanline = -1;
            frame_complete = true;
        }
    }
}

void ppu2C02::reset()
{
    control.regs = 0x00;
    mask.regs = 0x00;
    status.regs = 0x00;
    ppu_addr_latch = false;
    ppu_data_buffer = 0x00;
    //ppu_scroll_stuff = 0x00;
}

olc::Pixel& ppu2C02::getColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
    return palScreen[bus.read((palette<<2)+pixel+0x3F00)&0x3F];
}

olc::Sprite& ppu2C02::updatePaletteSprite(uint8_t i, uint8_t palette)
{
    // Loop through every tile
    // (16*16 tiles on screen) * (8*8 pixels per tile) = (128*128 pixels on screen)
    for (uint8_t tx=0;tx<16;tx++)
    {
        for (uint8_t ty=0;ty<16;ty++)
        {
            // Row offset into pattern table
            // Width is (single tile (which is 8 bytes * 2) * 16 (16 tiles in a row))
            uint16_t offset = ty*256 + tx*16;

            // Loop through every pixel in tile
            for (uint8_t py=0;py<8;py++)
            {
                // Get LSB and MSB for entire row of 8 pixels from pattern table
                // addr = (pattern table * sizeof(PatternTable) + py)
                uint8_t LSB = bus.read(i*0x1000 + offset + py);
                uint8_t MSB = bus.read(i*0x1000 + offset + py + 8);

                // Loop through every pixel in py
                for (uint8_t px=0;px<8;px++)
                {
                    // 0010 0111
                    // 1010 1010
                    //
                    // 0000 1000
                    // px = 3
                    uint8_t mask = 1 << (8-px);
                    uint8_t value = (uint8_t)((MSB & mask) >> (8-px)) + (uint8_t)((LSB & mask) >> (8-px));

                    // x = (tile * 8 (8 pixels per tile) + px (pixel offset into tile))
                    sprPatternTables[i].SetPixel(
                        tx * 8 + px,
                        ty * 8 + py,
                        getColorFromPaletteRam(palette, value)
                    );
                }
            }
        }
    }
    return sprPatternTables[i];
}

