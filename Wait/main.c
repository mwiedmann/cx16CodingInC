#include <stdio.h>
#include <cx16.h>
#include <peekpoke.h>

void wait(void)
{
    unsigned char start, next;
    
    __asm__(
        "jsr $FFDE\n"
        "sta 0"
    );

    start = *((unsigned char*)0);

    do {
        __asm__(
            "jsr $FFDE\n"
            "sta 0"
        );

        next = *((unsigned char*)0);
    } while(next == start);

    // printf("%u %u\n", start, next);
}

void main() {

    // Here is the FIXED address for the palette
    unsigned long paletteAddr = 0x1FA00;
    unsigned char i, color;
    
    color = 0;

    while(1) {
        // Point to the MapBase address so we can write to VRAM
        VERA.address = paletteAddr;
        VERA.address_hi = paletteAddr>>16;

        // Set the Increment Mode, turn on bit 4
        VERA.address_hi |= 0b10000;

        for (i=0; i<16; i++) {
            VERA.data0 = color;
            color++;
            VERA.data0 = color;
            color++;
        }

        // wait();
        
    }
}