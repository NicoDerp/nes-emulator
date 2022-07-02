
#include <iostream>
#include "bus.h"


int main()
{
    printf("Hello world!\n");

    Bus nes;

    unsigned int val = nes.read(0x0000);
    printf("Value: %d\n", val);

    return 0;
}

