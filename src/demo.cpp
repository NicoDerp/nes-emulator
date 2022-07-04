#define OLC_PGE_APPLICATION
//#include <iostream>
#include "olcPixelGameEngine.h"
//#include "bus.h"

class Emulator : public olc::PixelGameEngine
{
public:
    Emulator()
    {
        sAppName = "NES Emulator";
    }

public:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        return true;
    }

//private:
//    Bus nes;
};

int main()
{
    //printf("Starting emulator...\n");


    Emulator emu;
    if (emu.Construct(640, 480, 2, 2))
        emu.Start();

    return 0;
}

