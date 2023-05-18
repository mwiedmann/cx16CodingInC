#include <cx16.h>

#define WIDTH_BYTES_640_4BPP 640 / 2 // 320

void main() {
    // Our default Tile base address is too small to hold a bitmap
    // We will instead use VRAM Address 0
    unsigned long tileBaseAddr = 0;
    unsigned short x,y;
    unsigned char color = 0;

    // Bitmap mode
    // Color Depth 2 - 4 bpp
    VERA.layer1.config = 0b00000110;

    // Get bytes 16-11 of the new TileBase address
    // Set bit 0 to 1 (for 640 mode)
    VERA.layer1.tilebase = tileBaseAddr>>9 | 0b1;

    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // 320x240
    for (y=0; y<240; y++) {
        // Each byte covers 2 pixels
        for (x=0; x<WIDTH_BYTES_640_4BPP; x++) {
            VERA.data0 = color<<4 | color;
        }

        color++;
        if (color == 16) {
            color = 0;
        }
    }
}