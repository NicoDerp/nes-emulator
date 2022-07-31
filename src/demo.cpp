
#define OLC_PGE_APPLICATION

#include <iostream>
#include "olcPixelGameEngine.h"
#include "cpubus.h"
#include "helper.h"

class Emulator : public olc::PixelGameEngine
{
public:
    Emulator()
    {
        sAppName = "NES Emulator";
    }

public:
    void DrawCpu(int x, int y)
    {
        DrawString(x, y, "STATUS:", olc::WHITE);
        DrawString(x+64, y, "C", nes.cpu.getFlag(cpu6502::C) ? olc::GREEN : olc::RED);
        DrawString(x+80, y, "Z", nes.cpu.getFlag(cpu6502::Z) ? olc::GREEN : olc::RED);
	    DrawString(x+96, y, "I", nes.cpu.getFlag(cpu6502::I) ? olc::GREEN : olc::RED);
        DrawString(x+112, y, "D", nes.cpu.getFlag(cpu6502::D) ? olc::GREEN : olc::RED);
        DrawString(x+128, y, "B", nes.cpu.getFlag(cpu6502::B) ? olc::GREEN : olc::RED);
        DrawString(x+144, y, "U", nes.cpu.getFlag(cpu6502::U) ? olc::GREEN : olc::RED);
        DrawString(x+160, y, "V", nes.cpu.getFlag(cpu6502::V) ? olc::GREEN : olc::RED);
        DrawString(x+176, y, "N", nes.cpu.getFlag(cpu6502::N) ? olc::GREEN : olc::RED);
        DrawString(x, y+10, "PC: $" + hex(nes.cpu.pc, 4));
        DrawString(x, y+20, "A: $" + hex(nes.cpu.a, 2) + "[" + std::to_string(nes.cpu.a) + "]");
        DrawString(x, y+30, "X: $" + hex(nes.cpu.x, 2) + "[" + std::to_string(nes.cpu.x) + "]");
        DrawString(x, y+40, "Y: $" + hex(nes.cpu.y, 2) + "[" + std::to_string(nes.cpu.y) + "]");
        DrawString(x, y+50, "STACK PTR: $" + hex(nes.cpu.stackptr, 4));
    }

    void DrawPage(int x, int y, uint8_t page)
    {
        uint16_t addr = page<<8;
        for (int row=0;row<16;row++)
        {
            std::string s ="$"+hex(addr,4)+":";
            for (int col=0;col<16;col++)
            {
                s += " "+hex(nes.read(addr,true),2);
                addr++;
            }
            DrawString(x,y,s);
            y+=10;
        }
    }

    void DrawCode(int x, int y, int lines)
    {
        std::map<uint16_t,std::string>::iterator cur = asmMap.find(nes.cpu.pc);
        if (cur!=asmMap.end())
            DrawString(x, y, cur->second, olc::CYAN);
        cur++;
        for (uint8_t i=1;(i<lines>>1)&&(cur!=asmMap.end());i++,cur++)
        {
            DrawString(x, y+i*10, cur->second);
        }

        cur = asmMap.find(nes.cpu.pc);
        cur--;
        for (uint8_t i=1;(i<lines>>1)&&(cur!=asmMap.end());i++,cur--)
        {
            DrawString(x, y-i*10, cur->second);
        }
    }

    void DrawStack(int x, int y, uint8_t lines)
    {
        DrawString(x,y,"STACK:  " + std::to_string(0xFF-nes.cpu.stackptr));

        if ((0xFF-nes.cpu.stackptr) < lines)
            lines = 0xFF - nes.cpu.stackptr;

        for (uint8_t i=0;i<lines;i++)
        {
            uint16_t addr = 0x0200-(0xFF-nes.cpu.stackptr)+i;
            uint8_t val = nes.read(addr);

            std::string s = "$"+hex(addr,4)+": "+hex(val,2);

            DrawString(x,y+10*(i+1),s);
        }
    }

    bool OnUserCreate() override
    {
        // Load the cartridge
        cart = std::make_shared<Cartridge>("test_roms/blargg_cpu/official_only.nes");

        // Check if the image is valid
        if (!cart->imageValid())
        {
            printf("[ERROR] Failed to load image\n");
            return false;
        }

        // Insert cartridge into NES
        nes.insertCartridge(cart);

        // Disassemble code
        asmMap = nes.cpu.disassemble(0x0000, 0xFFFF);

        // Reset
        nes.reset();

        // For debug
        //nes.cpu.pc = 0xC000;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        totalElapsedTime += fElapsedTime;

        Clear(olc::DARK_GREY);

        if (running)
        {
            if (totalElapsedTime >= (1.0f/60.0f))
            {
                do {nes.clock();} while (!nes.ppu.frame_complete);
                nes.ppu.frame_complete = false;
                totalElapsedTime = 0.0f;
            }

            //for (uint8_t n=0;n<50;n++)
            //{
            //    if (nes.cpu.pc == 0xCD93)
            //    {
            //        running = false;
            //        break;
            //    }
            //    do {nes.clock();} while (!nes.cpu.complete());
            //}
        }

        if (GetKey(olc::Key::ESCAPE).bPressed)
        {
            return false;
        }
        else if (GetKey(olc::Key::M).bPressed)
        {
            for (uint16_t i=0;i<1000;i++)
            {
                do { nes.clock(); }
                while (!nes.cpu.complete());

                // Not sure why
                //do { nes.clock(); }
                //while (nes.cpu.complete());
            }
        }
        else if (GetKey(olc::Key::N).bPressed)
        {
            for (uint8_t i=0;i<100;i++)
            {
                do { nes.clock(); }
                while (!nes.cpu.complete());

                // Not sure why
                //do { nes.clock(); }
                //while (nes.cpu.complete());
            }
        }
        else if (GetKey(olc::Key::B).bPressed)
        {
            do { nes.clock(); }
            while (!nes.cpu.complete());

            // Not sure why
            do { nes.clock(); }
            while (nes.cpu.complete());

        }

        else if (GetKey(olc::Key::F).bPressed)
        {
            do { nes.clock(); }
            while (!nes.ppu.frame_complete);

            do { nes.clock(); }
            while (!nes.cpu.complete());

            nes.ppu.frame_complete = false;
        }
        else if (GetKey(olc::Key::R).bPressed)
        {
            nes.cpu.reset();
        }
        else if (GetKey(olc::Key::P).bPressed)
        {
            ++selectedPalette &= 0x7;
        }
        else if (GetKey(olc::Key::T).bPressed)
        {
            running = !running;
        }

        // Directly copied from javidx9
		const int nSwatchSize = 6;
		for (int p = 0; p < 8; p++) // For each palette
			for(int s = 0; s < 4; s++) // For each index
				FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340,
					nSwatchSize, nSwatchSize, nes.ppu.getColorFromPaletteRam(p, s));

        DrawRect(516 + selectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);

        DrawCpu(520, 2);
        DrawCode(520, 200, 26);

        //DrawPage(2, 2, 0x00);
        //DrawPage(2, 182, 0x80);
        //DrawStack(2, 200, 10);

        DrawSprite(516, 348, &nes.ppu.updatePaletteSprite(0, selectedPalette));
        DrawSprite(648, 348, &nes.ppu.updatePaletteSprite(1, selectedPalette));

        DrawSprite(0, 0, &nes.ppu.getScreen(), 2);

        //olc::Sprite& s = nes.ppu.updatePaletteSprite(0, selectedPalette);
        //for (uint8_t y=0;y<30;y++)
        //{
        //    for (uint8_t x=0;x<32;x++)
        //    {
        //        //printf("(%d,%d): 0x%s\n",x,y,hex((uint32_t)nes.ppu.bus.nametables[0][y*32+x],2).c_str());
        //        DrawString(x*16,y*16,hex((uint32_t)nes.ppu.bus.nametables[0][y*32+x],2));
        //        //uint8_t id = (uint32_t)nes.ppu.bus.nametables[0][y*32+x];
        //        //DrawPartialSprite(x*16,y*16,&s,(id&0x0F)<<3,((id>>4)&0x0F)<<3,8,8,2);
        //    }
        //}

        return true;
    }

private:
    CPUBus nes;
    std::shared_ptr<Cartridge> cart;
    std::map<uint16_t, std::string> asmMap;
    uint8_t selectedPalette = 0;
    bool running=false;
    float totalElapsedTime=0.0f;
};

int main()
{
    Emulator emu;
    if (emu.Construct(780, 480, 4, 4))
        emu.Start();

    return 0;
}

