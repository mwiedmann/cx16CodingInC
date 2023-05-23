#include "wait.h"
#include <cx16.h>

#define ANIMATION_SPEED 6 // Lower is faster

void main() {
    unsigned short i, a;
    unsigned short x = 0; // Start at x,y: 0,200
    unsigned short y = 200;
    unsigned char color;

    // VRAM address to store sprite images
    // We will start at 0 this time to give us some room
    unsigned long spriteGraphicAddress = 0;

    // VRAM address for sprite 1 (this is fixed)
    unsigned long sprite1Address = 0x1FC08;

    // Enable sprites
    VERA.display.video |= 0b01000000;

    // Create 16 animation frames of cycles colors
    // This will create a reverse waterfall animation
    // Each 64x64 pixels, 256 colors = 4k each
    // This will use a LOT of VRAM, 64k
    for (a=0; a<16; a++) {
        // Move 4k for each animation frame
        spriteGraphicAddress = a * 0x1000;

        VERA.address = spriteGraphicAddress;
        VERA.address_hi = spriteGraphicAddress>>16;
        // Set the Increment Mode, turn on bit 4
        VERA.address_hi |= 0b10000;

        color= a * 16;
        for (i=0; i<4096; i++) {
            VERA.data0 = color;
            if (i % 16 == 0) {
                color++;
            }
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
    VERA.data0 = x;
    VERA.data0 = x>>8;
    VERA.data0 = y;
    VERA.data0 = y>>8;
    VERA.data0 = 0b00001100; // Z-Depth=3, Sprite in front of layer 1
    VERA.data0 = 0b11110000; // 64x64 pixel image

    // Start at animation frame 0
    a=0;

    while (1) {
        for (i=0; i<ANIMATION_SPEED; i++) {
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
            x+=1;
            y+=1;

            if (x>639) {
                x = 0;
            }
            if (y>479) {
                y=0;
            }

            wait();
        }

        // Next animation frame (0-15)
        a++;
        if (a == 16) {
            a= 0;
        }

        // Change the animation frame
        // Point to sprite 1 so we can update it
        VERA.address = sprite1Address;
        VERA.address_hi = sprite1Address>>16;
        // Set the Increment Mode, turn on bit 4
        VERA.address_hi |= 0b10000;

        // The address for animation frame # "a"
        spriteGraphicAddress = a * 0x1000;

        // Graphic address bits 12:5
        VERA.data0 = spriteGraphicAddress>>5;
        // Graphic address bits 16:13
        // Careful when setting this byte that you don't
        // wipe out the "mode" bit 7
        VERA.data0 = 0b10000000 | spriteGraphicAddress>>13;
    }
}