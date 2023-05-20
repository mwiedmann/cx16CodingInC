#include <stdio.h>
#include <cx16.h>
#include <joystick.h>
#include "wait.h"

#define MAPBASE_TILE_COUNT 128*64

void main() {
    // Get an address for about the middle of the screen
    unsigned long mapBaseAddr = 0x1B000 + MAPBASE_TILE_COUNT + 80;
    unsigned char fore, back, joy;

    printf("USE THE LEFT/RIGHT CURSOR KEYS TO MOVE\n");

    // Configure the joysticks
    joy_install(cx16_std_joy);

    // Point to the MapBase address so we can write to VRAM
    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;

    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Start at colors 1 and 0
    fore = 1;
    back = 0;

    // Write out an @ character to the current MapBase addresses
    while(1) {
        VERA.data0 = 0; // Index for the @ character
        
        // Color index 0 for background (black)
        // Color index 1 for foreground (white)
        VERA.data0 = fore | back<<4;

        fore++;
        if (fore > 15) {
            // Skip color 0 for foreground so we get a good
            // variety of fore/back color combos.
            fore = 1;
        }

        back++;
        if (back > 15) {
            back = 0;
        }

        // Our new "wait" function to give us a 60hz heartbeat
        wait();

        do {
            joy = joy_read(0);

            // If LEFT, move the address back 2 bytes
            if (JOY_LEFT(joy)) {
                mapBaseAddr-= 2;
                break;
            // If RIGHT, move the address forward 2 bytes
            } else if (JOY_RIGHT(joy)) {
                mapBaseAddr+= 2;
                break;
            }
            // Stay in this loop until left/right is pressed
        } while(1);

        // Point to the new mapBaseAddress
        VERA.address = mapBaseAddr;
    }
}