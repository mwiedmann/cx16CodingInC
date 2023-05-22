#include "wait.h"
#include <cx16.h>

void main() {
    unsigned short i;
    unsigned short x = 0; // Start at x,y: 0,200
    unsigned short y = 200;
    unsigned char color;

    // VRAM address to store sprite image
    unsigned long spriteGraphicAddress = 0x1000; // 4k

    // VRAM address for sprite 1 (this is fixed)
    unsigned long sprite1Address = 0x1FC08;

    // Enable sprites
    VERA.display.video |= 0b01000000;

    // Create a 64x64 256 color image
    VERA.address = spriteGraphicAddress;
    VERA.address_hi = spriteGraphicAddress>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Use all 256 colors in a 64x64 block
    color=0;
    for (i=0; i<4096; i++) {
        VERA.data0 = color;
        if (i % 16 == 0) {
            color++;
        }
    }

    // Point to Sprite 1
    VERA.address = sprite1Address;
    VERA.address_hi = sprite1Address>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Configure Sprite 1

    // Graphic address bits 12:5
    VERA.data0 = spriteGraphicAddress>>5;
    // 256 color mode, and graphic address bits 16:13
    VERA.data0 = 0b10000000 | spriteGraphicAddress>>13;
    VERA.data0 = x; // X=100
    VERA.data0 = x>>8;
    VERA.data0 = y; // Y=100
    VERA.data0 = y>>8;
    VERA.data0 = 0b00001100; // Z-Depth=3, Sprite in front of layer 1
    VERA.data0 = 0b11110000; // 64x64 pixel image

    while (1) {
        // Update Sprite 1 X/Y Position
        // Point to Sprite 1 byte 2
        VERA.address = sprite1Address+2;
        VERA.address_hi = sprite1Address>>16;
        // Set the Increment Mode, turn on bit 4
        VERA.address_hi |= 0b10000;
        
        // Set the X and Y values
        VERA.data0 = x;
        VERA.data0 = x>>8;
        VERA.data0 = y;
        VERA.data0 = y>>8;

        // Move Sprite diagonally
        x+=2;
        y+=2;

        if (x>639) {
            x = 0;
        }
        if (y>479) {
            y=0;
        }

        wait();
    }
}