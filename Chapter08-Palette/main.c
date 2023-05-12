#include <cx16.h>

void main() {
    // Here is the FIXED address for the palette
    unsigned long paletteAddr = 0x1FA00;
    unsigned char i;
    unsigned char blue = 15; // Blue color

    // Point to the MapBase address so we can write to VRAM
    VERA.address = paletteAddr;
    VERA.address_hi = paletteAddr>>16;

    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Make the first 16 colors all shades of blue
    // The last color (15) will have blue=0 which is Black
    for (i=0; i<16; i++) {
        VERA.data0 = blue; // We are ignoring bits 7-4 (Green) they are set to 0
        VERA.data0 = 0; // Ignore Red as well (set to 0)

        blue--;
    }
}