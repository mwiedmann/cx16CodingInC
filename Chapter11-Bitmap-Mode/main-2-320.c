#include <cx16.h>

#define WIDTH_BYTES_320_2BPP 320 / 4 // 80

void main() {
    // Our default Tile base address is too small to hold a bitmap
    // We will instead use VRAM Address 0
    unsigned long tileBaseAddr = 0;
    unsigned short x,y;
    
    // Bitmap mode
    // Color Depth 1 - 2 bpp
    VERA.layer1.config = 0b00000101;

    // Get bytes 16-11 of the new TileBase address
    // Set bit 0 to 0 (for 320 mode), its already 0 in this address
    VERA.layer1.tilebase = tileBaseAddr>>9;

    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Uncomment these to get true 320x240
    // otherwise VERA will duplicate the screen and show extra (random) bytes from VRAM
    // VERA.display.hscale = 64;
    // VERA.display.vscale = 64;

    // 320x240
    for (y=0; y<240; y++) {
        // Each byte covers 4 pixels
        // so we only need to write out 320/4=80 bytes per row
        for (x=0; x<WIDTH_BYTES_320_2BPP; x++) {
            // Show 4 columns of colors across the screen
            VERA.data0 = x < 20
                ? 0 // Black
                : x < 40
                    ? 0b01010101 // White
                    : x < 60
                        ? 0b10101010 // Red
                        : 0b11111111; // Cyan
        }
    }
}