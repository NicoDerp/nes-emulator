
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
        /** Load program (assembled at https://www.masswerk.at/6502/assembler.html)

           *=$8000
           ldy #10
           loop:
           dey
           bne loop
           lda #$FF
           nop
           nop
           nop
           --------
           *=$8000
           sed
           lda #$28
           clc
           adc #$19
           nop
           nop
           nop
           --------
           *=$8000
           sed
           lda #$12
           clc
           sbc #$21
           nop
           nop
           nop
           --------
           *=$8000
           sed
           sec
           lda #$12
           sbc #$21

           sed
           sec
           lda #$21
           sbc #$34
           nop
           nop
           nop
           --------
           *=$8000

           lda #3
           sta $00
           lda #0

           ldx #5

           loop:
           adc $00
           dex
           bne loop

           lda #$DE
           sta $03
           lda #$AD
           sta $04
           lda #$BE
           sta $05
           lda #$EF
           sta $06

           nop
           nop
           nop
           --------


         */

        //std::stringstream ss;
        //ss << "A9 10 8D FE FF A9 80 8D FF FF 4C 0A 80 EA EA EA E6 00 58 40";
        /*
        uint16_t pgOffset = 0x8000;
        while (!ss.eof())
        {
            std::string b;
            ss >> b;
            nes.ram[pgOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
        }
        */

        cart = std::make_shared<Cartridge>("nestest.nes");

        nes.insertCartridge(cart);
        if (!cart->imageValid())
        {
            printf("[ERROR] Failed to load image\n");
            return false;
        }

        asmMap = nes.cpu.disassemble(0x0000, 0xFFFF);

        nes.write(0xFFFC, 0x04);
        nes.write(0xFFFD, 0xC0);

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
            do
            {
                nes.cpu.clock();
            }
            while (!nes.cpu.complete());
        }
        else if (GetKey(olc::Key::R).bPressed)
        {
            nes.cpu.reset();
        }
        else if (GetKey(olc::Key::I).bPressed)
        {
            nes.cpu.irq();
        }
        else if (GetKey(olc::Key::N).bPressed)
        {
            nes.cpu.nmi();
        }

        DrawCpu(448, 2);

        DrawString(448, 400, "CLOCK COUNT");
        DrawString(448, 410, std::to_string(nes.cpu.clockCount));

        DrawPage(2, 2, 0x00, 16);
        DrawPage(2, 182, 0x80, 16);
        DrawCode(448, 200, 26);

        DrawString(10, 370, "SPACE = Step Instruction    R = Reset    I = IRQ    N = NMI");

        return true;
    }

private:
    CPUBus nes;
    std::shared_ptr<Cartridge> cart;
    std::map<uint16_t, std::string> asmMap;
};

int main()
{
    //printf("Starting emulator...\n");


    Emulator emu;
    if (emu.Construct(640, 480, 4, 4))
        emu.Start();

    return 0;
}

