// Courtesy of CX16 Forum User "Manannan"

// Compiling this code requires a compiler option to load the bank config `-C cx16-bank.cfg`
// The cx16-bank.cfg is a file provided with the compiler

// Each bank can hold up to 8191 bytes of code. 
// Bank 0 is system reserved

#include <stdio.h>
#include <stdlib.h>
#include <cx16.h>
#include <cbm.h>

#define NO_CODE_BANKS 2  // Number of unique sections. 

// Mem addr 0 holds the current RAM Bank
#define BANK_NUM (*(unsigned char *)0x00)

// These symbols will exist as long as a related #pragma code-name (push, "BANKRAM0X") section exists
extern void _BANKRAM01_SIZE__[], _BANKRAM02_SIZE__[];

// Compile this code for Bank 2
#pragma code-name (push, "BANKRAM02")
void callBank2A() {
    printf("This code runs from Bank 2\n\n");
}

void callBank2B() {
    printf("This code ALSO runs from Bank 2\n\n");
}
#pragma code-name (pop)

// Back on the low RAM in this function
void foo() {
    // We are going to change the Bank so we call functions on Bank 2.
    // We need to store the previous Bank so we can switch back at the end.
    unsigned char previousBank = BANK_NUM;

    printf("Foo was called from a Bank 1 function\nbut that is ok\n\n");

    // Switch to Bank 2 before calling a function there
    BANK_NUM = 2;

    printf("Foo is calling functions on Bank2\n\n");
    callBank2A();
    callBank2B();

    // Restore the previous Bank
    BANK_NUM = previousBank;
}

// Compile this code for Bank 1
#pragma code-name (push, "BANKRAM01")
void callBank1() {
    printf("This code runs from Bank 1\nand calls a main function\n\n");
    foo();
}
#pragma code-name (pop)

void bankedRamInit() {
    int i;
    unsigned char fileName[32];
    unsigned char previousBank = BANK_NUM;

    // Load all Bank code files
    for (i = 1; i <= NO_CODE_BANKS; i++) {
        // One file will exist per code bank
        // The prefix is the name of your executable
        sprintf(fileName, "code.prg.0%x", i);
        printf("Loading code file %s\n", fileName);
        
        // Set the Bank we are loading into
        BANK_NUM = i;
        
        // Load the file into Banked RAM
        cbm_k_setnam(fileName);
        cbm_k_setlfs(0, 8, 0); // Skip the first 2 bytes of the file. They just hold the size in bytes.
        cbm_k_load(0, (unsigned short)BANK_RAM);
    }

    BANK_NUM = previousBank;
}

void main() {
    // Load the Bank code files into Banked RAM
    bankedRamInit();
    
    // Show that the "SIZE" variables have been populated
    // You can use these to know the size of the code loaded
    // but you can also get that from the load function
    printf("_BANKRAM01_SIZE__ %u\n", _BANKRAM01_SIZE__);
    printf("_BANKRAM02_SIZE__ %u\n", _BANKRAM02_SIZE__);

    // Our first function is on Bank 1
    // Set memory addr 0x0 to 1 to switch to Bank 1
    BANK_NUM = 1;

    // Call our function on Bank 1
    callBank1();
}
