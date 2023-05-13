#include <cx16.h>

#define MAPBASE_TILE_COUNT 128*64

void main() {
    // Here is the address we know Layer 1 MapBase is using
    unsigned long mapBaseAddr = 0x1B000;
    unsigned char color = 0;

    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i;

    VERA.layer1.config |= 0b1000;

    // Point to the MapBase address so we can write to VRAM
    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;

    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Write out @ characters to the entire MapBase
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        VERA.data0 = 0; // Index for the @ character
        
        // Color index 0 for background (black)
        // Color index 1 for foreground (white)
        VERA.data0 = color;

        // Don't change the color unless we are on screen (column < 80)
        // otherwise many of the colors are only off screen
        if (i % 128 < 80) {
            color++;
        }
    }
}