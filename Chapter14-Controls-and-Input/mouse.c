#include "wait.h"
#include <cx16.h>
#include <stdio.h>

void main() {
    unsigned short i;
    unsigned char color;

    // Address for sprite 0
    // The mouse pointer is stored here
    unsigned long sprite0Address = 0x1FC00;
    unsigned long mouseGraphicAddress;
    unsigned long spData0, spData1;

    // Use 2 pointers to get the mouse data returned
    // from the _mouse_get Kernal Function
    unsigned short *mouseX = (unsigned short *)0x2;
    unsigned short *mouseY = (unsigned short *)0x4;

    // You can use global or static var for this
    static unsigned char mouseButtons;

    // *** Configure the mouse ***

    // Call the screen_mode Kernal Function
    // Loads the X/Y registers with current screen resolution
    // The mouse_config function uses these to set its resolution
    // You can change it to restrict the mouse to certain areas
    asm("jsr $FF5F");

    // Load register A with 1 to enable the mouse pointer
    asm("lda #1");

    // Call mouse_config Kernal Function
    asm("jsr $FF68");

    // Wait a cycle for the mouse to fully activate
    wait();

    // Sprite 0
    VERA.address = sprite0Address;
    VERA.address_hi = sprite0Address>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Get some sprite 0 bytes (contains address of graphic)
    spData0 = VERA.data0;
    spData1 = VERA.data0;

    // Get a full VRAM address for the mouse graphic
    mouseGraphicAddress = ((spData1 & 0b1111)<<13) | spData0<<5;

    printf("Mouse graphic stored at VRAM address 0x%lX\n", mouseGraphicAddress);

    // Should be
    // 128 = 0b10000000
    // 137 - 0b1000 1001

    // 1 0011 0000 000 00000
    // Address 16-5: 0b1 00110000 000 00000 - 0x13000

    // Check and print the mouse location and button status each loop
    while (1) {
        // Register X holds the zero-page memory address where
        // mouse_get will store 4 bytes of mouse x,y data
        // bytes 0-1=X position, bytes 2-3=Y position
        asm("ldx #2");

        // Call the mouse_get Kernal Function
        asm("jsr $FF6B");
        
        // mouseX/Y now hold the X/Y positions
        // but Register A holds the button statuses. Get it.
        asm("sta %v", mouseButtons);

        printf("X,Y: %u, %u Buttons: %u\n", *mouseX, *mouseY, mouseButtons);

        // If holding a mouse button, change the mouse pointer to wild colors
        if (mouseButtons) {
            // This will set a starting random color of sorts 
            // (based on the mouse Y position)
            color = *mouseY;

            // Overwrite the mouse point graphic in VRAM
            VERA.address = mouseGraphicAddress;
            VERA.address_hi = mouseGraphicAddress>>16;
            // Set the Increment Mode, turn on bit 4
            VERA.address_hi |= 0b10000;

            // Change the mouse pointer rainbow of colors
            // Mouse pointer is a 16x16 pixel, 256 color (8 bpp) graphic
            // so we need 256 bytes
            for (i=0; i<256; i++) {
                VERA.data0 = color++;
            }
        }

        // Wait 1 second so we don't spam the screen too quickly
        // NOTE: This is an enhancement to our "wait" code
        waitCount(60);
    }
}