
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
    uint8_t data = 0x00;

    addr &= 0x7; // Mirror

    // For all readonly stuff. It reads even though it's an write-only adress.
    // Thats because rdonly is pretty much only for debug so it's okay

    if (addr==0x0) // PPUCTRL
    {
        // Write-only
        if (rdonly)
            data = control.reg;
    }
    else if (addr==0x1) // PPUMASK
    {
        // Write-only
        if (rdonly)
            data = mask.reg;
    }
    else if (addr==0x2) // PPUSTATUS
    {
        if (rdonly)
        {
            data = status.reg;
        }
        else
        {
            //printf("VBLANK CLEARED\n");
            //printf("REG: %d, VBLANK: %s\n", status.reg&0xE0, (status.vblank ? "TRUE" : "FALSE"));
            // Top three bits of status + some ppu noise
            data = (status.reg&0xE0) | (ppu_data_buffer&0x1F);

            status.vblank = false;
            ppu_addr_latch = false;
        }

    }
    else if (addr==0x3) // OAMADDR
    {
        // Write-only
    }
    else if (addr==0x4) // OAMDATA
    {
        // TODO
        //data = oam.bytes[oam_addr++];
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
        if (!rdonly)
        {
            // Screen is turned off or VBLANK
            //        if (status.vblank || (!mask.render_spr && !mask.render_bgr))

            data = ppu_data_buffer;
            ppu_data_buffer = bus.read(vram_addr.reg);

            // PPUDATA read is delayed except for reading palettes
            if (vram_addr.reg >= 0x3F00)
                data = ppu_data_buffer;

            vram_addr.reg += (control.increment_mode ? 32 : 1); // im*31+1
        }
    }
    else
    {
        // Can't get here, but I don't trust myself enough :o
        printf("[WARNING] Unreachable location in ppu2C02::cpuRead reached %d. Bug in mirroring\n",addr);
    }
    return data;
}

void ppu2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x7; // Mirror

    if (addr==0x0) // PPUCTRL
    {
        control.reg = data;
        tram_addr.nametable_x = control.nametable_x;
        tram_addr.nametable_y = control.nametable_y;
    }
    else if (addr==0x1) // PPUMASK
    {
        mask.reg = data;
    }
    else if (addr==0x2) // PPUSTATUS
    {
        // Read-only
    }
    else if (addr==0x3) // OAMADDR
    {
        // TODO
        //oam_addr = data;
    }
    else if (addr==0x4) // OAMDATA
    {
        // TODO
        //oam.bytes[oam_addr] = data;
        //oam_addr++;
    }
    else if (addr==0x5) // PPUSCROLL
    {
        // First write is x, second is y
        // Then split x and y into coarse and fine
        if (ppu_addr_latch == 0)
        {
            // Write x
            fine_x = data & 0x07;
            tram_addr.coarse_x = data >> 3;

        }
        else
        {

            // Write y
            tram_addr.fine_y = data & 0x07;
            tram_addr.coarse_y = data >> 3;
        }

        ppu_addr_latch = !ppu_addr_latch;
    }
    else if (addr==0x6) // PPUADDR
    {
        if (ppu_addr_latch == 0)
        {
            // Write high (this is first)
            tram_addr.reg = (((uint16_t)data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
        }
        else
        {
            // Write low (this is second)
            tram_addr.reg |= data;
            vram_addr.reg = tram_addr.reg;
            //vram_addr = tram_addr;
        }

        ppu_addr_latch = !ppu_addr_latch;
    }
    else if (addr==0x7) // PPUDATA
    {
        bus.write(vram_addr.reg, data);
        vram_addr.reg += (control.increment_mode ? 32 : 1); // im*31+1
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
    //printf("1(%d, %d): %d\n", cycle, scanline, status.vblank);
    //sprScreen.SetPixel(cycle-1, scanline, palScreen[(rand()%2) ? 0x3F : 0x30]);

    // (https://www.nesdev.org/wiki/PPU_scrolling)
    // The visible scanlines (including Pre-render line which is not visible)
    if (scanline >= -1 && scanline <= 239)
    {
        if (scanline == 0 && cycle == 0)
        {
            // Odd-frame skipped
            cycle = 1;
        }

        // Pre-render line!
        if (scanline == -1)
        {
            // Clear VBLANK
            if (cycle == 1)
            {
                //printf("NOT VBALNK\n");
                status.vblank = false;
            }
        }

        // Fetch next tile info (not cycle==0, idle)
        if ((1 <= cycle && cycle <= 256) || (321 <= cycle && cycle <= 336))
        {
            if (mask.render_bgr)
            {
                bg_shifter_pat_low <<= 1;
                bg_shifter_pat_high <<= 1;
                bg_shifter_attr_low <<= 1;
                bg_shifter_attr_high <<= 1;
            }

            // Inc hori(v) (even in cycle 256 vert(v))
            if ((cycle&0x07) == 0)
            {
                if (mask.render_bgr || mask.render_spr)
                {
                    // If overflow toggle bit 10 (nametable_x)
                    if (vram_addr.coarse_x == 31)
                    {
                        vram_addr.nametable_x = ~vram_addr.nametable_x;
                        vram_addr.coarse_x = 0;
                    }
                    else
                    {
                        vram_addr.coarse_x++;
                    }
                }
            }

            // NT byte
            if ((cycle&0x07) == 1)
            {
                bg_shifter_pat_low = (bg_shifter_pat_low & 0xFF00) | bg_next_lsb;
                bg_shifter_pat_high = (bg_shifter_pat_high & 0xFF00) | bg_next_msb;
                bg_shifter_attr_low = (bg_shifter_attr_low & 0xFF00) | ((bg_next_attr & 0b01) ? 0xFF : 0x00);
                bg_shifter_attr_high = (bg_shifter_attr_high & 0xFF00) | ((bg_next_attr & 0b01) ? 0xFF : 0x00);

                bg_next_id = bus.read(0x2000 | (vram_addr.reg & 0x0FFF));
                //printf("READING NT BYTE FROM 0x%s: 0x%s\n", hex(0x2000|(vram_addr.reg&0x0FFF),4).c_str(), hex(bg_next_id,2).c_str());
            }

            // AT byte
            if ((cycle&0x07) == 3)
            {
                //bg_next_attr = bus.read(0x23C0
                //                        | (vram_addr.nametable_y << 11)
                //                        | (vram_addr.nametable_x << 10)
                //                        | ((vram_addr.coarse_y >> 2) << 3)
                //                        | (vram_addr.coarse_x >> 2));

                //   1 1 1 x x
                // 1 1 1 . . .

                bg_next_attr = bus.read(0x23C0
                                        | (vram_addr.coarse_x >> 2)
                                        | ((vram_addr.coarse_y & 0x07) << 1)
                                        | (vram_addr.nametable_x << 10)
                                        | (vram_addr.nametable_y >> 11));

                if (vram_addr.coarse_x & 0x02)
                    bg_next_attr >>= 2;

                if (vram_addr.coarse_y & 0x02)
                    bg_next_attr >>= 4;

                bg_next_attr &= 0x03;
            }

            // Low BG tile byte
            if ((cycle&0x07) == 5)
            {
                bg_next_lsb = bus.read(((uint16_t)control.bgr_pattern_addr << 12)
                                       + ((uint16_t)bg_next_id << 4)
                                       + ((uint16_t)vram_addr.fine_y) + 0);
            }

            // High BG tile byte
            if ((cycle&0x07) == 7)
            {
                bg_next_msb = bus.read(((uint16_t)control.bgr_pattern_addr << 12)
                                       + ((uint16_t)bg_next_id << 4)
                                       + ((uint16_t)vram_addr.fine_y) + 8);
            }
        }
    }

    // Inc vert(v)
    // End of scanline, increment y
    if ((cycle == 256) && (mask.render_bgr || mask.render_spr))
    {
        // If overflow increment coarse_y
        if (vram_addr.fine_y >= 0x7)
        {
            // If overflow toggle bit 10 (nametable_y)
            if (vram_addr.coarse_y >= 29)
            {
                vram_addr.nametable_y = ~vram_addr.nametable_y;
                vram_addr.coarse_y = 0;
            }
            // End of nametable, beginning of attribute memory.
            // Wrap around
            else if (vram_addr.coarse_y >= 31)
            {
                vram_addr.coarse_y = 0;
            }
            else
            {
                vram_addr.coarse_y++;
            }

            vram_addr.fine_y = 0;
        }
        else
        {
            vram_addr.fine_y++;
        }
    }


    // hori(v) = hori(t)
    if (cycle == 257)
    {
        if (mask.render_bgr || mask.render_spr)
        {
            vram_addr.coarse_x = tram_addr.coarse_x;
            vram_addr.nametable_x = tram_addr.nametable_x;
        }

        bg_shifter_pat_low = (bg_shifter_pat_low & 0xFF00) | bg_next_lsb;
        bg_shifter_pat_high = (bg_shifter_pat_high & 0xFF00) | bg_next_msb;
        bg_shifter_attr_low = (bg_shifter_attr_low & 0xFF00) | ((bg_next_attr & 0b01) ? 0xFF : 0x00);
        bg_shifter_attr_high = (bg_shifter_attr_high & 0xFF00) | ((bg_next_attr & 0b01) ? 0xFF : 0x00);

    }

    // vert(v) = vert(t)
    if ((scanline == -1) && (280 <= cycle && cycle <= 304))
    {
        if (mask.render_bgr || mask.render_spr)
        {
            vram_addr.fine_y = tram_addr.fine_y;
            vram_addr.coarse_y = tram_addr.coarse_y;
            vram_addr.nametable_y = tram_addr.nametable_y;
        }
    }

    // Unused NT fetches (?)
    if ((cycle == 337) || (cycle == 339))
    {
        // Should it update shifters?? Not sure
        //bg_shifter_pat_low = (bg_shifter_pat_low & 0xFF00) | bg_next_lsb;
        //bg_shifter_pat_high = (bg_shifter_pat_high & 0xFF00) | bg_next_msb;
        //bg_shifter_attr_low = (bg_shifter_attr_low & 0xFF00) | ((bg_next_attr & 0b01) ? 0xFF : 0x00);
        //bg_shifter_attr_high = (bg_shifter_attr_high & 0xFF00) | ((bg_next_attr & 0b01) ? 0xFF : 0x00);

        bg_next_id = bus.read(0x2000 | (vram_addr.reg & 0x0FFF));

    }

    if (scanline == 240)
    {
        // Nothing happens
        // Post-render line!
    }

    // VBLANK
    if ((scanline == 241) && (cycle == 1))
    {
        // printf("VBLANK ON\n");
        status.vblank = true;

        if (control.vblank_nmi)
            nmi = true;
    }


    // Get the palette and pattern stuff
    uint8_t bg_pixel = 0x00;
    uint8_t bg_palette = 0x00;

    if (mask.render_bgr)
    {
        // Default the MSB of the shifter.
        // But the fine_x can control an extra offset
        // So MSB >> fine_x
        uint16_t bitmask = (1 << 15) >> fine_x;

        // > 0 to not have to bitshift >> 15.
        // But that works too
        uint8_t p0_pixel = (bg_shifter_pat_low & bitmask) > 0;
        uint8_t p1_pixel = (bg_shifter_pat_high & bitmask) > 0;
        bg_pixel = (p1_pixel << 1) | p0_pixel;

        uint8_t bg_pal0 = (bg_shifter_attr_low & bitmask) > 0;
        uint8_t bg_pal1 = (bg_shifter_attr_high & bitmask) > 0;
        bg_palette = (bg_pal1 << 1) | bg_pal0;
    }

    // Enhance colors
    olc::Pixel pixel = getColorFromPaletteRam(bg_palette, bg_pixel);

    if (mask.enhance_red)
        pixel.r = std::min((int)(pixel.r * 1.7f), 255);

    if (mask.enhance_grn)
        pixel.g = std::min((int)(pixel.g * 1.7f), 255);

    if (mask.enhance_blu)
        pixel.b = std::min((int)(pixel.b * 1.7f), 255);


    // Finally!!! Draw
    sprScreen.SetPixel(cycle-1, scanline, pixel);

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
    //printf("2(%d, %d): %d\n", cycle, scanline, status.vblank);
}

void ppu2C02::reset()
{
    control.reg = 0x00;
    mask.reg = 0x00;
    status.reg = 0x00;
    ppu_addr_latch = false;
    ppu_data_buffer = 0x00;

    vram_addr.reg = 0x0000;
    tram_addr.reg = 0x0000;

    cycle = 0;
    scanline = 0;

    fine_x = 0x00;

    bg_next_id = 0x00;
    bg_next_attr = 0x00;
    bg_next_lsb = 0x00;
    bg_next_msb = 0x00;

    bg_shifter_pat_low = 0x0000;
    bg_shifter_pat_high = 0x0000;
    bg_shifter_attr_low = 0x0000;
    bg_shifter_attr_high = 0x0000;

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
                    uint8_t mask = 1 << (7-px);
                    uint8_t value = (uint8_t)((MSB & mask) > 0)<<1 | (uint8_t)((LSB & mask) > 0);

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

