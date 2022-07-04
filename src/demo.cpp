
#define OLC_PGE_APPLICATION

#include <iostream>
#include "olcPixelGameEngine.h"
#include "bus.h"

class Emulator : public olc::PixelGameEngine
{
public:
    Emulator()
    {
        sAppName = "NES Emulator";
    }

public:
    std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

    void DrawCpu(int x, int y)
    {
        DrawString(x, y, "STATUS:", olc::WHITE);
        DrawString(x+64, y, "C", nes.cpu.getFlag(cpu6502::C) ? olc::GREEN : olc::RED);
        DrawString(x+80, y, "Z", nes.cpu.getFlag(cpu6502::Z) ? olc::GREEN : olc::RED);
        DrawString(x+96, y, "C", nes.cpu.getFlag(cpu6502::C) ? olc::GREEN : olc::RED);
	    DrawString(x+112, y, "I", nes.cpu.getFlag(cpu6502::I) ? olc::GREEN : olc::RED);
        DrawString(x+128, y, "D", nes.cpu.getFlag(cpu6502::D) ? olc::GREEN : olc::RED);
        DrawString(x+144, y, "B", nes.cpu.getFlag(cpu6502::B) ? olc::GREEN : olc::RED);
        DrawString(x+160, y, "U", nes.cpu.getFlag(cpu6502::U) ? olc::GREEN : olc::RED);
        DrawString(x+176, y, "V", nes.cpu.getFlag(cpu6502::V) ? olc::GREEN : olc::RED);
        DrawString(x+192, y, "N", nes.cpu.getFlag(cpu6502::N) ? olc::GREEN : olc::RED);
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
        /** Load program (assembled at https://www.masswerk.at/6502/assembler.html)

           *=$8000
           ldy #10
           loop:
           dey
           bne loop
           lda $FF
           nop
           nop
           nop

         */

        std::stringstream ss;
        ss << "A0 0A 88 D0 FD A5 FF EA EA EA";
        uint16_t pgOffset = 0x8000;
        while (!ss.eof())
        {
            std::string b;
            ss >> b;
            nes.ram[pgOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
        }

        // Reset vector to program start (0x8000)
        nes.ram[0xFFFC] = 0x00; // Low byte
        nes.ram[0xFFFD] = 0x80; // High byte

        nes.cpu.reset();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::DARK_GREY);

        if (GetKey(olc::Key::SPACE).bPressed)
        {
            do
            {
                nes.cpu.clock();
            }
            while (!nes.cpu.complete());
        }

        if (GetKey(olc::Key::R).bPressed)
        {
            nes.cpu.reset();
        }

        if (GetKey(olc::Key::I).bPressed)
        {
            nes.cpu.irq();
        }

        if (GetKey(olc::Key::N).bPressed)
        {
            nes.cpu.nmi();
        }

        DrawCpu(448, 2);

        DrawString(448, 70, "INSTRUCTION");
        DrawString(448, 80, nes.cpu.getInsName());
        DrawString(448, 90, hex(nes.cpu.fetched, 2));

        DrawString(448, 110, "CLOCK COUNT");
        DrawString(448, 120, std::to_string(nes.cpu.clockCount));

        DrawString(448, 140, "PROGRAM COUNTER");
        DrawString(448, 150, std::to_string(nes.cpu.pc));

        DrawPage(0, 0, 0x00, 16);

        DrawString(10, 370, "SPACE = Step Instruction    R = Reset    I = IRQ    N = NMI");

        return true;
    }

private:
    Bus nes;
};

int main()
{
    //printf("Starting emulator...\n");


    Emulator emu;
    if (emu.Construct(640, 480, 2, 2))
        emu.Start();

    return 0;
}

