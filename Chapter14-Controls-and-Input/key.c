#include <stdio.h>

unsigned char keycode;

void main() {
    while(1) {
        asm("jsr $FFE4");
        asm("sta %v", keycode);

        if (keycode) {
            printf("PETSCII Code %u\n", keycode);
        }
    }
}