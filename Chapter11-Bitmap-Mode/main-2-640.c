#include <cx16.h>

#define WIDTH_BYTES_640_2BPP 640 / 4 // 160

void main() {
    // Our default Tile base address is too small to hold a bitmap
    // We will instead use VRAM Address 0
    unsigned long tileBaseAddr = 0;
    unsigned short x,y;

    // Bitmap mode
    // Color Depth 1 - 2 bpp
    VERA.layer1.config = 0b00000101;

    // Get bytes 16-11 of the new TileBase address
    // Set bit 0 to 1 (for 640 mode)
    VERA.layer1.tilebase = tileBaseAddr>>9 | 0b1;

    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // 640x480
    for (y=0; y<480; y++) {
        // Each byte covers 4 pixels
        // so we only need to write out 640/4=160 bytes per row
        for (x=0; x<WIDTH_BYTES_640_2BPP; x++) {
            // Show 8 columns of colors across the screen
            VERA.data0 = x < 20
                ? 0 // Black
                : x < 40
                    ? 0b01010101 // White
                    : x < 60
                        ? 0b10101010 // Red
                        : x < 80
                            ? 0b11111111 // Cyan
                            : x < 100
                                ? 0b00010001 // Black/White
                                : x < 120
                                    ? 0b10111011 // Red/Cyan
                                    : x < 140
                                        ? 0b00110011 // Black/Cyan
                                        : 0b01100110; // White/Red
        }
    }
}