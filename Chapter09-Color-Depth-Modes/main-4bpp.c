#include <cx16.h>

#define MAPBASE_TILE_COUNT 128*64

// An arrow pointing up/right
unsigned char tileData[32] = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00010001, 0b00010001, 0b00010000,
    0b00000000, 0b00000000, 0b01000011, 0b00010000,
    0b00000000, 0b00000100, 0b00110100, 0b00010000,
    0b00000000, 0b10001110, 0b01000000, 0b00010000,
    0b00001000, 0b11101000, 0b00000000, 0b00010000,
    0b10100110, 0b10000000, 0b00000000, 0b00000000,
    0b01101010, 0b00000000, 0b00000000, 0b00000000
};

void main() {
    // Our default Tile and Map Base addresses
    unsigned long tileBaseAddr = 0x1F000;
    unsigned long mapBaseAddr = 0x1B000;
    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i;
    unsigned char flip, pOffset;

    // Turn on Color Depth 2 - 4 bpp
    VERA.layer1.config |= 0b10;
    
    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Create the arrow tile
    // Pull the bytes from the tileData array
    for (i=0; i<32; i++) {
        VERA.data0 = tileData[i];
     }

    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Draw the arrow tile in all combinations of flips and palette offsets
    flip = 0;
    pOffset = 0;
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        VERA.data0 = 0;
        VERA.data0 =
            pOffset<<4 | // Shift the offset to bits 7-4
            flip<<2; // Shift the flip to bits 3-2

        // Go through all 4 flip values
        flip++;
        if (flip==4) {
            flip=0;
        }

        // Go through all 16 palette offset values
        pOffset++;
        if (pOffset==16) {
            pOffset=0;
        }
    }   
}