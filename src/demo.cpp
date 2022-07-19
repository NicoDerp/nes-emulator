
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

    void DrawPage(int x, int y, uint8_t page, uint8_t size)
    {
        uint16_t addr = page<<8;
        for (int row=0;row<size;row++)
        {
            std::string s ="$"+hex(addr,4)+":";
            for (int col=0;col<size;col++)
            {
                s += " "+hex(nes.read(addr),2);
                addr++;
            }
            DrawString(x,y,s);
            y+=10;
        }
    }

    bool OnUserCreate() override
    {
        // Load the cartridge
        cart = std::make_shared<Cartridge>("nestest.nes");

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
        nes.cpu.reset();

        return true;
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

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::DARK_GREY);

        if (GetKey(olc::Key::ESCAPE).bPressed)
        {
            return false;
        }
        else if (GetKey(olc::Key::SPACE).bPressed)
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

        DrawCpu(520, 2);

        DrawString(520, 350, "CLOCK COUNT");
        DrawString(520, 360, std::to_string(nes.cpu.clockCount));

        //        DrawPage(2, 2, 0x00, 16);
        //DrawPage(2, 182, 0x80, 16);

        DrawSprite(0, 0, &nes.ppu.getScreen(), 2);

        DrawCode(520, 200, 26);

        //        DrawString(10, 370, "SPACE = Step Instruction   F = Complete Single Frame    R = Reset");

        return true;
    }

private:
    CPUBus nes;
    std::shared_ptr<Cartridge> cart;
    std::map<uint16_t, std::string> asmMap;
};

int main()
{
    Emulator emu;
    if (emu.Construct(780, 480, 4, 4))
        emu.Start();

    return 0;
}

