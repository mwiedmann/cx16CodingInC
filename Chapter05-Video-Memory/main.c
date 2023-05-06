#include <stdio.h>

void main() {
    // Registers (pointers to memory locations)
    // Need the (unsigned char/short *) cast to avoid compiler warning
    unsigned short *addrLoMid = (unsigned short *)0x9F20;
    unsigned char *addrHi = (unsigned char *)0x9F22;
    unsigned char *data0 = (unsigned char *)0x9F23;

    // Use a long (4 bytes) to hold the full VRAM address
    unsigned long vramAddr = 0x1A077;

    // For loops and holding data
    unsigned char i;
    unsigned char list[10];

    // Pull out the Lo/Mid (1st and 2nd) bytes
    // Because addrLoMid is a 2 byte data type, it will just get 2 bytes
    *addrLoMid = vramAddr;

    // Shift >> the Lo/Mid bytes out.
    // This leaves the Hi (3rd) byte for us to grab
    // Because addHi is a 1 byte data type, it will just get 1 byte
    *addrHi = vramAddr>>16;

    // Set the Increment Mode, turn on bit 4
    *addrHi |= 0b10000;

    // Write out 10 bytes (1 to 10) to DATA0
    for (i=1; i<=10; i++) {
        *data0=i;
    }

    // The VRAM address will have changed thanks to our Increment mode
    // Set the AddrLo back to the start and read those 10 bytes back in
    *addrLoMid = vramAddr;

    // Read in 10 bytes (hopefully 1 to 10) into a buffer for display later
    for (i=1; i<=10; i++) {
        list[i] = *data0;
    }

    // IMPORTANT: We print last because printing to the screen will change the VRAM address
    // Don't do any printing in the middle of VRAM code or you will be confused when the
    // address is changing in the middle of your code!

    printf(
        "The address we started parsing is 0x%lX\n"
        "addrHi is register 0x%X and has 0x%X (1 for VRAM addr and Inc Mode)\n"
        "addrLo/Mid is registers are at 0x%X and point to 0x%04X\n"
        "The addrLo should have incremented 10 bytes to 0x81\n"
        ,vramAddr
        ,addrHi, *addrHi
        ,addrLoMid, *addrLoMid
    );    

    printf("We should get the numbers 1-10 back from VRAM\n");
    for (i=1; i<=10; i++) {
        printf("%u\n", list[i]);
    }
}