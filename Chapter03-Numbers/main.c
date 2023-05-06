#include <stdio.h>

void main() {
    unsigned char mem = 255;

    mem &= 0b11111110;
    printf("mem %u\n", mem);

    printf("First some Hex numbers\n");
    printf("This is 1 %u\n", 0x1);
    printf("This is also 1 %u\n", 0x01);
    printf("This is also 1, leading 0's don't matter %u\n", 0x001);

    printf("This is 128 %u\n", 0x80);
    printf("This is 129 %u\n", 0x81);

    printf("This is 257, we are using more than 1 byte now %u\n", 0x101);
    printf("This is 65535, the max 16bit number %u\n", 0xFFFF);


    printf("Now some Binary numbers\n");
    printf("This is 1 %u\n", 0b1);
    printf("This is also 1 %u\n", 0b01);
    printf("This is also 1, leading 0's don't matter %u\n", 0b001);

    printf("This is 128 %u\n", 0b10000000);
    printf("This is 129 %u\n", 0b10000001);

    printf("This is 257, we are using more than 1 byte now %u\n", 0b100000001);
    printf("This is 65535, the max 16bit number %u\n", 0b1111111111111111);
}