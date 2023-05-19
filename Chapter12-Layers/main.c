#include <cx16.h>

// We will use 16x16 Tiles so the screen will be 40x30 (this is 64x32 tiles)
#define MAPBASE_TILE_COUNT 64*32

#define TILE_SIZE 256 // 16x16 Tiles in 8 bpp mode = 256 bytes
#define NUM_TILES 24
#define TILE_BASE_SIZE TILE_SIZE * NUM_TILES // 6,144

// MapBase will use 2 bytes x (64 x 32 Tiles) = 4kb (4096)
#define MAPBASE_BYTE_SIZE MAPBASE_TILE_COUNT * 2

// We can put our TileBase as Addr 0
#define TILE_BASE_ADDR 0

// Start the MapBase at 6,144 to give the TileBase enough room for 24 tiles
// WARNING: Remember the chapter on MapBase, the address must be a multiple of 512 
#define LAYER0_MAPBASE_ADDR TILE_BASE_SIZE

// Start the L1 MapBase after the L0 MapBase
// Just add MAPBASE_BYTE_SIZE since that is how much mem the L0 MapBase uses
#define LAYER1_MAPBASE_ADDR LAYER0_MAPBASE_ADDR + MAPBASE_BYTE_SIZE

// Switch this to 0 to make L1 ALL transparent so you can see all of L0
#define DRAW_LAYER_1 1

// This program will draw both layers
// L0 - Will be tiles of rotating colors
// L1 - Will alternate between transparent and solid white columns
//      You will see the L0 checkered tiles show through the transparent columns
void main() {
    unsigned long tileBaseAddr = TILE_BASE_ADDR;
    unsigned long l0MapBaseAddr = LAYER0_MAPBASE_ADDR;
    unsigned long l1MapBaseAddr = LAYER1_MAPBASE_ADDR;
    unsigned char c;
    unsigned short i;

    // Enable both layers
    VERA.display.video |= 0b00110000;

    // Layers can share a tilebase (use the same tiles)
    // Get bytes 16-11 of the new TileBase address
    // ALSO Set Tile W/H (bits 0/1) to 1 for 16 pixel tiles
    VERA.layer0.tilebase = tileBaseAddr>>9 | 0b11;
    VERA.layer1.tilebase = tileBaseAddr>>9 | 0b11;

    // Get bytes 16-9 of the MapBase addresses and set on both layers
    VERA.layer0.mapbase = l0MapBaseAddr>>9;
    VERA.layer1.mapbase = l1MapBaseAddr>>9;

    // With 16 pixel tiles, we don't need as many tiles
    // Only 640/16 = 40, 480/16=30 (40x30 tile resolution now)
    // Set the Map Height=0 (32), Width=1 (64)
    // Set Color Depth to 8 bpp mode
    VERA.layer1.config = 0b00010011;
    VERA.layer0.config = 0b00010011;

    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Create 16 tiles, each with a different color (0-15)
    // Tile 0 will be all transparent color 0
    for (c=0; c<24; c++) {
        for (i=0; i<TILE_SIZE; i++) {
            VERA.data0 = c;
        }
    }

    // Point to the L0 MapBase
    VERA.address = l0MapBaseAddr;
    VERA.address_hi = l0MapBaseAddr>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // L0 uses all the colors between 2-23
    c = 2;
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        VERA.data0 = c;
        VERA.data0 = 0;

        c++;
        if (c == 24) {
            c=2;
        }
    }

    // Point to the L1 MapBase
    VERA.address = l1MapBaseAddr;
    VERA.address_hi = l1MapBaseAddr>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Alternate between columns of transparent and white
    // You will see L0 through the transparent columns
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        VERA.data0 = DRAW_LAYER_1
            ? i % 2 == 0 ? 0 : 1
            : 0;
        VERA.data0 = 0;
    }
}