#include <stdio.h>
#include "wait.h"

unsigned char status1, status2;

void decodeJoystickBytes() {
    // Here are the bits for each button/pad on the joystick
    /*
    .A, byte 0:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
            SNES | B | Y |SEL|STA|UP |DN |LT |RT |

    .X, byte 1:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
            SNES | A | X | L | R | 1 | 1 | 1 | 1 |

    We aren't using this one but we could if we wanted to check
    if the joystick was actually present or not
    .Y, byte 2:
        $00 = joystick present
        $FF = joystick not present
    */

    // IMPORTANT: Bits will be 0 when that button/pad is pressed, NOT 1.
    // This means all bits are 1 when nothing is pressed.
    // So to check a bit, we do an AND (&) but NEGATE (!) to flip it

    // We won't check every button, but here are a few
    // Byte from A register - status1
    if (!(status1 & 0b10000000)) {
        printf("B-BTN ");
    }
    if (!(status1 & 0b01000000)) {
        printf("Y-BTN ");
    }
    if (!(status1 & 0b00000001)) {
        printf("RIGHT ");
    }
    if (!(status1 & 0b00000010)) {
        printf("LEFT ");
    }

    // Byte from X register - (status 2
    if (!(status2 & 0b10000000)) {
        printf("A-BTN ");
    }
    if (!(status2 & 0b01000000)) {
        printf("X-BTN ");
    }
    // You can add more
    printf("--\n");
}

void main() {
    // You pass params to kernal functions using
    // memory addresses starting at 2
    // Create a pointer for the "joystick number" param
    // at address 2
    unsigned char *kernalParam0 = (unsigned char *)2;

    while (1) {
        // Check joystick 0 (the default)
        *kernalParam0 = 0;

        // Call the "joystick_get" Kernal function at 0xFF56
        asm("jsr $FF56");

        // Get the status bytes from the A and X registers
        asm("STA %v", status1);
        asm("STX %v", status2);

        // Examine the bytes we got and see which buttons/pads
        // are currently being pressed
        decodeJoystickBytes();

        // Wait 1 second so we don't spam the screen too quickly
        // NOTE: This is an enhancement to our "wait" code
        waitCount(60);
    }
}