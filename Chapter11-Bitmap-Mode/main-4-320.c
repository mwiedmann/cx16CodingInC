#include <cx16.h>

#define WIDTH_BYTES_320_4BPP 320 / 2 // 160

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
    // Set bit 0 to 0 (for 320 mode), its already 0 in this address
    VERA.layer1.tilebase = tileBaseAddr>>9;

    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Need these to get true 320x240
    // otherwise VERA will duplicate the screen and show extra (random) bytes from VRAM
    VERA.display.hscale = 64;
    VERA.display.vscale = 64;

    // 320x240
    for (y=0; y<240; y++) {
        // Each byte covers 2 pixels
        // so we only need to write out 320/2=160 bytes per row
        for (x=0; x<WIDTH_BYTES_320_4BPP; x++) {
            VERA.data0 = color<<4 | color;
        }

        color++;
        if (color == 16) {
            color = 0;
        }
    }
}