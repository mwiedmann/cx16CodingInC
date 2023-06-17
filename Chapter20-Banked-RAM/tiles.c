#include <cx16.h>
#include <cbm.h>
#include "wait.h"

#define MAPBASE_TILE_COUNT 128*64

// Our default Tile and Map Base addresses
#define TILEBASE_ADDR 0x1F000
#define MAPBASE_ADDR 0x1B000

// A pointer to Memory location 0
// which holds the number of the current RAM Bank
#define BANK_NUM (*(unsigned char *)0x00)

// Pointers to mem 2/4/6 which are "virtual registers"
// for Kernal Functions, used to pass/get data
#define R0 (*(unsigned short *)0x02)
#define R1 (*(unsigned short *)0x04)
#define R2 (*(unsigned short *)0x06)

#define FLAME_BANK 1
#define SNOW_BANK 2

// Each tile file is 256 bytes
// The cbm_k_load function can return info about how much data was read
// if you are dealing with variable sized files
#define TILE_FILESIZES 256

void loadFileToBankedRAM(unsigned char *filename, unsigned char bank) {
    // Set the RAM Bank we are loading into
    BANK_NUM = bank;

    // See the chapter on Files for info on these cbm file functions
    cbm_k_setnam(filename);
    cbm_k_setlfs(0, 8, 2);

    // Reminder, first param of cbm_k_load of "0" means load into system memory.
    // WE WANT THIS ONE because Banked RAM is system memory (at address 0xA000)

    // BANK_RAM is a #define provided by cx16.h
    // as a pointer to 0xA000 (which is where Bank RAM starts)
    cbm_k_load(0, (unsigned short)BANK_RAM);
}

void copyBankedRAMToVRAM(unsigned long vramAddressTo, unsigned char bank, unsigned short numBytes) {
    // We will be writing to the tileBase address
    VERA.address = vramAddressTo;
    VERA.address_hi = vramAddressTo>>16;
    VERA.address_hi |= 0b10000; // Increment mode

    // Set the RAM Bank we are loading from
    BANK_NUM = bank;

    // Virtual Registers for the Kernal Function
    R0 = (unsigned short)BANK_RAM; // Read from the Banked RAM starting Address 0xA000
    R1 = 0x9F23; // Write to the VERA.data0 memory location
    R2 = numBytes; // Write this number of bytes

    // Call the memory_copy Kernal Function
    __asm__("jsr $FEE7");
}

void main() {    
    // Note we need a `short` here because there are more than 255 tiles
    unsigned short i;
    
    // Turn on Color Depth 3 - 8 bpp
    VERA.layer1.config |= 0b11;

    // 16x16 pixel tiles
    VERA.layer1.tilebase |= 0b11;

    loadFileToBankedRAM("flame.bin", FLAME_BANK);
    loadFileToBankedRAM("snow.bin", SNOW_BANK);
   
    VERA.address = MAPBASE_ADDR;
    VERA.address_hi = MAPBASE_ADDR>>16;
    // Always set the Increment Mode, turn on bit 4
    VERA.address_hi |= 0b10000;

    // Draw the flame tile on all tile spaces
    for (i=0; i<MAPBASE_TILE_COUNT; i++) {
        VERA.data0 = 0;
        VERA.data0 = 0;
    }

    i=1;

    // Keep looping and swap the tile every 2 seconds
    while(1) {
        // Copy the image from Banked RAM into VRAM
        // so it will display that tile
        copyBankedRAMToVRAM(TILEBASE_ADDR, i, TILE_FILESIZES);

        // Wait 1 second
        waitCount(60);
        
        // Go to next Bank (which holds next tile image)
        i++;
        if (i>2) {
            i=1;
        }
    }
}