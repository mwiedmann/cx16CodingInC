#include "wait.h"
#include <6502.h>
#include <cx16.h>
#include <stdio.h>

#define SPRITE1_X_START 0
#define SPRITE2_X_START 500

#define IRQ_HANDLER_STACK_SIZE 8
unsigned char irqHandlerStack[IRQ_HANDLER_STACK_SIZE];

void configSprite(unsigned char spriteNum, unsigned long spriteGraphicAddress,
    unsigned short x, unsigned short y) {
    unsigned long spriteAddress = 0x1FC00 + (spriteNum * 8);

    // Point to Sprite
    VERA.address = spriteAddress;
    VERA.address_hi = spriteAddress>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Configure Sprite

    // Graphic address bits 12:5
    VERA.data0 = spriteGraphicAddress>>5;
    // 256 color mode, and graphic address bits 16:13
    VERA.data0 = 0b10000000 | spriteGraphicAddress>>13;
    VERA.data0 = x;
    VERA.data0 = x>>8;
    VERA.data0 = y;
    VERA.data0 = y>>8;
    VERA.data0 = 0b00011100; // Collision mask 0001, Z-Depth=3, Sprite in front of layer 1
    VERA.data0 = 0b11110000; // 64x64 pixel image
}

void moveSprite(unsigned char spriteNum, unsigned short x, unsigned short y) {
    unsigned long spriteAddress = 0x1FC00 + (spriteNum * 8);

    // Update Sprite 1 X/Y Position
    // Point to Sprite byte 2
    VERA.address = spriteAddress+2;
    VERA.address_hi = spriteAddress>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;
    
    // Set the X and Y values
    VERA.data0 = x;
    VERA.data0 = x>>8;
    VERA.data0 = y;
    VERA.data0 = y>>8;
}

unsigned char irqHandler() {
    // This handler may be called for other IRQs (like VSYNC)
    // See if there is a collision
    if (VERA.irq_flags & 0b100) {
        // Clear the collision IRQ by writing to the SPRCOL (bit 2) in the ISR
        VERA.irq_flags = 0b100; 
        return IRQ_HANDLED;
    }

    return IRQ_NOT_HANDLED;
}

void main() {
    unsigned short i;
    unsigned short x1 = SPRITE1_X_START; // Start at x,y: 0,200
    unsigned short y1 = 200;
    unsigned short x2 = SPRITE2_X_START; // Start at x,y: 500,200
    unsigned short y2 = 200;
    unsigned char color;

    // VRAM address to store sprite image
    unsigned long spriteGraphicAddress = 0x1000; // 4k

    // VRAM address for sprite 1 (this is fixed)
    unsigned long sprite1Address = 0x1FC08;
    unsigned long sprite2Address = 0x1FC10;

    // Enable sprites
    VERA.display.video |= 0b01000000;
    
    // Enable sprite collisions
    VERA.irq_enable |= 0b00000100; // 0b00000100;

    // Setup the IRQ handler for sprite collisions
    set_irq(&irqHandler, irqHandlerStack, IRQ_HANDLER_STACK_SIZE);

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

    configSprite(1, spriteGraphicAddress, x1, y1);
    configSprite(2, spriteGraphicAddress, x2, y2);

    while (1) {
        moveSprite(1, x1, y1);
        moveSprite(2, x2, y2);
        
        x1+=2;
        x2-=2;

        wait();

        // Check the collision bits to see if there was a collision
        // and who collided
        if (VERA.irq_flags & 0b11110000) {
            x1 = SPRITE1_X_START;
            x2 = SPRITE2_X_START;
        }
    }
}