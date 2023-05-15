#include <cx16.h>

#define MAPBASE_TILE_COUNT 64*64

void main() {
    // Our default Tile and Map Base addresses
    unsigned long tileBaseAddr = 0x1F000;
    unsigned long mapBaseAddr = 0x1B000;
    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i;

    // Map height = 64
    // Map width = 64
    // Color Depth 1 - 2 bpp
    VERA.layer1.config = 0b01010001;
    
    // Set tile widths to 16 pixels
    // This gives us 16x8 "wide" tiles
    VERA.layer1.tilebase |= 0b1;

    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Wide tiles require more memory
    // Create 2 simple tiles
    // Tile 1 - Vertical stripes
    for (i=0; i<32; i++) {
        VERA.data0 = 0b00010000;
    }

    // Tile 2 - Horizontal stripes
    for (i=0; i<8; i++) {
        VERA.data0 = 0;
    }
    for (i=0; i<4; i++) {
        VERA.data0 = 0xFF;
    }
    for (i=0; i<8; i++) {
        VERA.data0 = 0;
    }
    for (i=0; i<4; i++) {
        VERA.data0 = 0xFF;
    }
    for (i=0; i<8; i++) {
        VERA.data0 = 0;
    }

    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Alternate between the 2 tiles
    for (i=0; i<MAPBASE_TILE_COUNT / 2; i++) {
        // Tile 1
        VERA.data0 = 0;
        VERA.data0 = 0;

        // Tile 2
        VERA.data0 = 1;
        VERA.data0 = 0;
    }   
}