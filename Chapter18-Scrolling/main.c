#include <cx16.h>
#include <joystick.h>
#include "wait.h"
#include <6502.h>
#include <stdio.h>
#include <stdlib.h>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define MAPBASE_TILE_WIDTH 64
#define MAPBASE_TILE_HEIGHT 32
#define MAPBASE_TILE_COUNT MAPBASE_TILE_WIDTH * MAPBASE_TILE_HEIGHT

#define FIELD_TILE_WIDTH 64
#define FIELD_TILE_HEIGHT 30
#define FIELD_TILE_COUNT FIELD_TILE_WIDTH * FIELD_TILE_HEIGHT

#define FIELD_PIXEL_WIDTH FIELD_TILE_WIDTH * TILE_WIDTH
#define FIELD_PIXEL_HEIGHT FIELD_TILE_HEIGHT * TILE_HEIGHT

#define SCROLL_SPEED 3
#define BALL_SPEED 2

// VRAM addresses
#define TILEBASE 0x10000
#define L0_MAPBASE 0
#define SPRITE_IMAGE_START L0_MAPBASE + (MAPBASE_TILE_COUNT * 2)

// Sprites
#define BALL_SPRITE_IMAGE SPRITE_IMAGE_START

// Colors
#define FLOOR_COLOR 0
#define GRID_COLOR 19
#define WALL_COLOR 8
#define BALL_COLOR 4

#define SPRITE1_ATTR 0x1FC08

#define SPRITE_COUNT 32

typedef struct BallData {
    short x;
    short y;
    short xDir;
    short yDir;
} BallData;

BallData balls[SPRITE_COUNT];

void createSprites() {
    unsigned long ballSpriteImageAddress = BALL_SPRITE_IMAGE;
    unsigned long ballSpriteAttributes;
    unsigned short i;
    unsigned char s;

    // Create a ball image
    VERA.address = ballSpriteImageAddress;
    VERA.address_hi = ballSpriteImageAddress>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Solid tile for ball
    for (i=0; i<256; i++) {
        VERA.data0 = BALL_COLOR;
    }
    
    // Configure Sprites
    ballSpriteAttributes = SPRITE1_ATTR;
    VERA.address = ballSpriteAttributes;
    VERA.address_hi = ballSpriteAttributes>>16;
    // Set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    for (s=0; s < SPRITE_COUNT; s++) {
        balls[s].x = rand()>>5;
        balls[s].y = rand()>>6;
        balls[s].xDir = ((unsigned char)rand()) < 128 ? 1 : -1;
        balls[s].yDir = ((unsigned char)rand()) < 128 ? 1 : -1;

        // Graphic address bits 12:5
        VERA.data0 = ballSpriteImageAddress>>5;
        // 256 color mode, and graphic address bits 16:13
        VERA.data0 = 0b10000000 | ballSpriteImageAddress>>13;
        VERA.data0 = 0;
        VERA.data0 = 0;
        VERA.data0 = 0;
        VERA.data0 = 0;
        VERA.data0 = 0b00001100; // Z-Depth=3, Sprite in front of layer 1
        VERA.data0 = 0b01010000; // 64x64 pixel image
    }
}

void drawField() {
    unsigned long tileBaseAddr = TILEBASE;
    unsigned long mapBaseAddr = L0_MAPBASE;
    unsigned short i;

    // Point to the TileBase address so we can write to VRAM
    VERA.address = tileBaseAddr;
    VERA.address_hi = tileBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Create the floor tile, 16x16 pixels=256 pixels, Tile 0
    for (i=0; i<256; i++) {
        VERA.data0 = i < 16 || i >= (255-16) || i % 16 == 0 || (i+1) % 16 == 0 ? GRID_COLOR : FLOOR_COLOR;
    }

    // Corner wall tile
    for (i=0; i<256; i++) {
        VERA.data0 = i < 64 || i % 16 < 3
            ? WALL_COLOR
            : i >= (255-16) || (i+1) % 16 == 0 ? GRID_COLOR : FLOOR_COLOR;
    }

    // Left/Right wall tile
    for (i=0; i<256; i++) {
        VERA.data0 = i % 16 < 3
            ? WALL_COLOR
            : i < 16 || i >= (255-16) || (i+1) % 16 == 0 ? GRID_COLOR : FLOOR_COLOR;
    }

    // Up/Down wall tile
    for (i=0; i<256; i++) {
        VERA.data0 = i < 64
            ? WALL_COLOR
            : i >= (255-16) || i % 16 == 0 || (i+1) % 16 == 0 ? GRID_COLOR : FLOOR_COLOR;
    }

    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Fill layer 0 with the floor tiles
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        // Which tile to display?
        VERA.data0 = 
            // Corners
            i == 0 || i == FIELD_TILE_WIDTH - 1 || i == FIELD_TILE_WIDTH * (FIELD_TILE_HEIGHT-1) || i == FIELD_TILE_COUNT-1
            ? 1
            // Left/Right
            : i % FIELD_TILE_WIDTH == 0 || i % FIELD_TILE_WIDTH == FIELD_TILE_WIDTH - 1
                ? 2
                // Top/Bottom
                : i < FIELD_TILE_WIDTH || i > FIELD_TILE_WIDTH * (FIELD_TILE_HEIGHT-1)
                    ? 3 
                    : 0; // Empty field cells

        // Does it need to be flipped?
        VERA.data0 =
            // Corners
            i == FIELD_TILE_WIDTH - 1
            ? 0b00000100 // Flip H
            : i == FIELD_TILE_WIDTH * (FIELD_TILE_HEIGHT-1)
                ? 0b00001000 // Flip V
                : i == FIELD_TILE_COUNT-1
                    ? 0b00001100 // Flip Both

            // Left/Right
            : i % FIELD_TILE_WIDTH == FIELD_TILE_WIDTH - 1
                ? 0b00000100 // Flip H
            
            // Up Down
            : i > FIELD_TILE_WIDTH * (FIELD_TILE_HEIGHT-1)
                ? 0b00001000 // Flip V
            : 0; // No flipping needed
    }
}

void initGraphics() {
    unsigned long tileBaseAddr = TILEBASE;
    unsigned long mapBaseAddr = L0_MAPBASE;

    // Enable layer 0 only. VGA mode, enable sprites
    VERA.display.video = 0b11010001;

    // Turn on Color Depth 3 - 8 bpp
    // Tile W/H 64x32
    VERA.layer0.config = 0b00010011;
    
    // Set tile widths to 16 pixels
    // Also set the tilebase VRAM address
    // This gives us 16x16 tiles
    VERA.layer0.tilebase = tileBaseAddr>>9 | 0b11;

    // Set the mapbase VRAM address
    VERA.layer0.mapbase = mapBaseAddr>>9;
}

void main() {
    unsigned long ballSpriteAttributes;

    unsigned char joy, s;
    short scrollX, scrollY;

    initGraphics();
    drawField();
    createSprites();

    // Must do this once to setup joystick support
    joy_install(cx16_std_joy);

    scrollX=0;
    scrollY=0;
    
    while(1) {
        // Read joystick 0
        joy = joy_read(0);

        if (JOY_LEFT(joy)) {
            scrollX-=SCROLL_SPEED;
        } else if (JOY_RIGHT(joy)) {
            scrollX+=SCROLL_SPEED;
        }

        if (JOY_UP(joy)) {
            scrollY-=SCROLL_SPEED;
        } else if (JOY_DOWN(joy)) {
            scrollY+=SCROLL_SPEED;
        }

        // Uncomment this to limit scrolling to within the field
        // if (scrollX > FIELD_PIXEL_WIDTH - 640) {
        //     scrollX = FIELD_PIXEL_WIDTH - 640;
        // } else if (scrollX < 0) {
        //     scrollX = 0;
        // }

        // if (scrollY > FIELD_PIXEL_HEIGHT - 480) {
        //     scrollY = FIELD_PIXEL_HEIGHT - 480;
        // } else if (scrollY < 0) {
        //     scrollY = 0;
        // }

        for (s=0; s<SPRITE_COUNT; s++) {
            // Move Ball diagonally
            balls[s].x+=BALL_SPEED * balls[s].xDir;
            balls[s].y+=BALL_SPEED * balls[s].yDir;

            if (balls[s].x > FIELD_PIXEL_WIDTH - TILE_WIDTH) {
                balls[s].x = FIELD_PIXEL_WIDTH - TILE_WIDTH;
                balls[s].xDir*= -1;
            } else if (balls[s].x < 0) {
                balls[s].x = 0;
                balls[s].xDir*= -1;
            }

            if (balls[s].y > FIELD_PIXEL_HEIGHT - TILE_HEIGHT) {
                balls[s].y = FIELD_PIXEL_HEIGHT - TILE_HEIGHT;
                balls[s].yDir*= -1;
            } else if (balls[s].y < 0) {
                balls[s].y = 0;
                balls[s].yDir*= -1;
            }

            // Point to the current sprite s
            ballSpriteAttributes = SPRITE1_ATTR + (s*8);

            // Update the Ball (Sprite s) X/Y Position
            // Point to Sprite s byte 2
            VERA.address = ballSpriteAttributes+2;
            VERA.address_hi = ballSpriteAttributes>>16;
            // Set the Increment Mode, turn on bit 4
            VERA.address_hi |= 0b10000;
            
            // Set the X and Y values
            VERA.data0 = balls[s].x - scrollX;
            VERA.data0 = (balls[s].x - scrollX)>>8;
            VERA.data0 = balls[s].y - scrollY;
            VERA.data0 = (balls[s].y - scrollY)>>8;  
        }

        VERA.layer0.hscroll = scrollX;
        VERA.layer0.vscroll = scrollY;

        wait();
    }
}