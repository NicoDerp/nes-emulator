
#include "helper.h"

std::string hex(uint32_t n, uint8_t d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
};

void print_binary(int number, int num_digits) {
    printf("0b");
    int digit;
    for(digit = num_digits - 1; digit >= 0; digit--) {
        printf("%c", number & (1 << digit) ? '1' : '0');
    }
    printf("\n");
};

