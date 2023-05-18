#include <cx16.h>

void main() {
    // Our default Tile base address is too small to hold a bitmap
    // We will instead use VRAM Address 0
    unsigned long tileBaseAddr = 0;
    unsigned short x,y;
    unsigned char color = 0;

    // Bitmap mode
    // Color Depth 3 - 8 bpp
    VERA.layer1.config = 0b00000111;

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

    // 320x240 - Show 1 color per row
    // This will get us through 240 colors
    for (y=0; y<240; y++) {
        // Each byte covers 1 pixel
        for (x=0; x<320; x++) {
            VERA.data0 = color;
        }

        // This 1 byte value will wrap back to 0 after 255
        color++;
    }
}