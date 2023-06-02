#include <cx16.h>
#include <joystick.h>
#include "wait.h"
#include <6502.h>
#include <stdio.h>

#define IRQ_HANDLER_STACK_SIZE 64
unsigned char irqHandlerStack[IRQ_HANDLER_STACK_SIZE];

#define MAPBASE_TILE_COUNT 128*64
#define SPEED 3

void main() {
    // Here is the address we know Layer 1 MapBase is using
    unsigned long mapBaseAddr = 0x1B000;
    unsigned char fore, back;
    unsigned char joy;
    short x,y;

    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i;

    // Point to the MapBase address so we can write to VRAM
    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;

    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Start at colors 1 and 0
    fore = 1;
    back = 0;

    // Write out @ characters to the entire MapBase
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        if (i<128 || i > (MAPBASE_TILE_COUNT - 128) || i%128 == 0 || i%128==127) {
            VERA.data0 = 1;
        } else {
            VERA.data0 = 32; // Space
        }
        
        // Color index 0 for background (black)
        // Color index 1 for foreground (white)
        VERA.data0 = fore | back<<4;

        fore++;
        if (fore > 15) {
            // Skip color 0 for foreground so we get a good
            // variety of fore/back color combos.
            fore = 3; //1;
        }

        back++;
        if (back > 15) {
            back = 1; //0
        }
    }

    // Must do this once to setup joystick support
    joy_install(cx16_std_joy);

    x=0;
    y=0;

    while(1) {
        // Read joystick 0
        joy = joy_read(0);

        if (JOY_LEFT(joy)) {
            x-=SPEED;
        } else if (JOY_RIGHT(joy)) {
            x+=SPEED;
        }

        if (JOY_UP(joy)) {
            y-=SPEED;
        } else if (JOY_DOWN(joy)) {
            y+=SPEED;
        }

        if (x > 4095) {
            x-= 4095;
        } else if (x < 0) {
            x+=4095;
        }

        if (y > 4095) {
            y-= 4095;
        } else if (y < 0) {
            y+=4095;
        }

        VERA.layer1.hscroll = x;
        VERA.layer1.vscroll = y;

        wait();
    }
}