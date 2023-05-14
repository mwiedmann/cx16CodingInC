#include <cx16.h>

#define MAPBASE_TILE_COUNT 128*64

void main() {
    // Our Tile Base address
    // We will create 4 tiles
    // Each tile is 8x8 pixels * 2 bpp = 64 pixels * 2 bpp = 128 bits = 16 bytes
    // We will create 4 tiles = 64 bytes
    unsigned long tileBaseAddr = 0x1F000; // 0x10000; // 0x1F000; // 0; //Charset is READONLY

    // MapBase is always 2 bytes per tile
    // We have 128*64 (columns * rows) * 2 bytes = 16kb
    // We can just start after the tileBase's tiles
    unsigned long mapBaseAddr = 0x1B000; // 512; //     0b00000001;
    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i,r,c;


    // Turn on Color Depth 1 - 2 bpp
    // We know that the Color Depth starts at 0, so we can just OR with 0b1
    // If the Color Depth is currently unknown, then you have to use 2 operations
    // First AND with 0b11111100 to clear the Color Depth, then OR with 0b1 to set it to 1
    // VERA.layer1.config = (VERA.layer1.config & 0b11111100) | 0b1;
    VERA.layer1.config |= 0b1;
    
    // CHARSET is READONLY, so we need a new VRAM address to hold tiles
    VERA.layer1.tilebase = tileBaseAddr>>9;
    VERA.layer1.mapbase = mapBaseAddr>>9;

    // // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    for (i=0; i<16; i++) {
        VERA.data0 = 0b10101010;
    }
    for (i=0; i<16; i++) {
        VERA.data0 = 0b11111111;
    }

    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;
    // // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    for (r=0; r<60; r++) {
        for (c=0; c<128; c++) {
            VERA.data0 = (r+c) % 2;
            VERA.data0 = 0;
        }
    }
    // for (i=0; i<MAPBASE_TILE_COUNT; i++) {
    //     VERA.data0 = 0;
    //     VERA.data0 = ;
    // }
}