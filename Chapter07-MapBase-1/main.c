#include <cx16.h>

#define MAPBASE_TILE_COUNT 128 //*64

void main() {
    // Here is the address we know Layer 1 MapBase is using
    unsigned long mapBaseAddr = 0x1EB00;
    unsigned char fore, back;

    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i;

    // Point to the MapBase address so we can write to VRAM
    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;

    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Start at colors 1 and 0
    fore = 1;
    back = 0;

    // Write out @ characters to the entire MapBase
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        VERA.data0 = 0; // Index for the @ character
        
        // Color index 0 for background (black)
        // Color index 1 for foreground (white)
        VERA.data0 = fore | back<<4;

        fore++;
        if (fore > 15) {
            // Skip color 0 for foreground so we get a good
            // variety of fore/back color combos.
            fore = 1;
        }

        back++;
        if (back > 15) {
            back = 0;
        }
    }
}