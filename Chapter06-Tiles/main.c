#include <stdio.h>
#include <cx16.h>

void main() {
    // Our Tile Base address in binary (0x1F000)
    unsigned long tileBaseAddr = 0b11111000000000000;
    unsigned char i;

    printf("0x9F29 DC_VIDEO Register %X\n", VERA.display.video);
    printf("0x9F36 L1_CONFIG Register %X\n", VERA.layer1.config);
    printf("0x9F36 L1_TILEBASE Register %X\n", VERA.layer1.tilebase);

    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;

    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Change the @ character to a solid block (all bits on)
    for (i=0; i<8; i++) {
        VERA.data0 = 0b11111111;
    }

    // change the 'a' character to horizontal lines
    for (i=0; i<8; i++) {
        VERA.data0 = 0b10101010;
    }

    // change the 'b' character to vertical lines
    for (i=0; i<4; i++) {
        VERA.data0 = 0b11111111;
        VERA.data0 = 0;
    }
}