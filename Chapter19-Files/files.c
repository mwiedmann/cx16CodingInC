#include <cx16.h>
#include <cbm.h>
#include <string.h>

#define MAPBASE_TILE_COUNT 128*64

// Our default Tile and Map Base addresses
unsigned long tileBaseAddr = 0x1F000;
unsigned long mapBaseAddr = 0x1B000;
unsigned char *filename = "flame.bin";

void loadTileCMB() {
    // These 3 functions are basic wrappers for the Kernal Functions

    // You have to first set the name of the file you are working with.
    cbm_k_setnam(filename);

    // Next you setup the LFS (Logical File) for the file
    // First param is the Logical File Number
    //   Use 0 if you are just loading the file
    //   You can use other values to keep multiple files open
    // Second param is the device number
    //   The SD Card on the CX16 is 8
    // The last param is the Secondary Address
    // 0 - File has the 2 byte header, but skip it
    // 1 - File has the 2 byte header, use it
    // 2 - File does NOT have the 2 byte header
    cbm_k_setlfs(0, 8, 2);

    // Finally, load the file somewhere into RAM or VRAM
    // First param of cbm_k_load means:
    //   0, loads into system memory.
    //   1, perform a verify.
    //   2, loads into VRAM, starting from 0x00000 + the specified starting address.
    //   3, loads into VRAM, starting from 0x10000 + the specified starting address.
    // Second param is the 16 bit address 
    cbm_k_load(3, tileBaseAddr);
}

void loadTileAssembly() {
    // We need some "static" vars to be able to pass values to "asm"
    static unsigned char aReg, xReg, yReg;

    // See the loadTileCMB function for descriptions of the params
    // for these kernal functions as they are the same

    // SETNAM Kernal Function
    // Need to load registers with the address of the filename
    // and length of the filename
    xReg = (unsigned char)filename;
    yReg = (unsigned short)filename>>8;
    aReg = strlen(filename);
    asm("lda %v", aReg);
    asm("ldx %v", xReg);
    asm("ldy %v", yReg);
    asm("jsr $FFBD");

    // SETLFS Kernal Function
    asm("lda #0");
    asm("ldx #8");
    asm("ldy #2");
    asm("jsr $FFBA");

    // LOAD Kernal Function
    // Load registers with the tilebase address
    xReg = tileBaseAddr;
    yReg = tileBaseAddr>>8;
    asm("lda #3");
    asm("ldx %v", xReg);
    asm("ldy %v", yReg);
    asm("jsr $FFD5");
}

void main() {    
    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i;
    
    // Turn on Color Depth 3 - 8 bpp
    VERA.layer1.config |= 0b11;

    // 16x16 pixel tiles
    VERA.layer1.tilebase |= 0b11;

    // Load the tile from a file
    // Make sure to use all lowercase name here
    // BUT, the file itself should have all uppercase
    // Unclear why but I run into issues unless I follow this pattern
    
    // There are 2 examples here, try them both:
    // - assembly, calling the kernal functions
    // - cbm.h - using the cbm_k_ C functions (which just use the same kernals function)
   
    loadTileAssembly();
    // loadTileCMB(); // Uncomment to try the C functions, result should be the same
   
    VERA.address = mapBaseAddr;
    VERA.address_hi = mapBaseAddr>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Draw the flame tile on all tile spaces
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        VERA.data0 = 0;
        VERA.data0 = 0;
    }
}