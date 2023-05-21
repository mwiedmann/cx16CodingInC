# Controls and Input
You wouldn't have much of a game without a way for players to control things. The CX16 has a mouse, supports SNES style joypads, and of course has a full keyboard for input as well. The standard C library functions for reading keyboard input all work, but those are more appropriate for console, STDIN/OUT operations, and text based programs. For faster, lower level keyboard input we have another Kernal Function we can use. Mouse and Joystick input have their own Kernal Functions and also are well supported by cc65 libraries.

## Joystick
We had a quick peek at the cc65's joystick support in a previous chapter, but let's build it from scratch. There is a Kernal Function `joystick_get $FF56` that we can use to check the state of any connected joysticks. See the [Kernal](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md#function-name-joystick_get) docs for full details.

We pass it the joystick number (starting at 0) that we want to check. It will return 3 bytes worth of info about the yes/no (being pressed) state of all the buttons and pads. The 3rd byte says whether or not the joystick is present at all:

```
    .A, byte 0:      | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
                SNES | B | Y |SEL|STA|UP |DN |LT |RT |

    .X, byte 1:      | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
                SNES | A | X | L | R | 1 | 1 | 1 | 1 |
    .Y, byte 2:
                $00 = joystick present
                $FF = joystick not present
```

Here is the C code to get the status of Joystick 0:

```C
// You pass params to kernal functions using
// memory addresses starting at 2
// Create a pointer for the "joystick number" param
// at address 2
unsigned char *kernalParam0 = (unsigned char *)2;

// Check joystick 0 (the default)
*kernalParam0 = 0;

// Call the "joystick_get" Kernal function at 0xFF56
asm("jsr $FF56");

// Get the status bytes from the A and X registers
asm("STA %v", status1);
asm("STX %v", status2);

```

At this point you can check the individual bits as detailed above to see which buttons/pads are being pressed.

>IMPORTANT NOTE: Bits will be 0 when the button/pad is being pressed, NOT 1. This is backwards from what you'd normally expect. I'm sure it has a good reason but I'm not sure why. You can check bits with AND (&) as usual and just NEGATE (!) the result at the end.

### cc65 joystick.h
cc65 has joystick functionality if you if choose to use it. Since cc65 supports many systems, it's joystick functions are more generic and don't support all of the buttons on a SNES controller. Instead, it only has 4 directions and 4 buttons. This may be enough for most game, but if you need FULL support of all buttons, you will have to use the Kernal Function directly.

See [joystick.h](https://github.com/cc65/cc65/blob/master/include/joystick.h) for the Macros and functions you can use.

To use cc65's joystick functions:

```C
#include <cx16.h>
#include <joystick.h>

unsigned char joy;

// Must do this once to setup joystick support
joy_install(cx16_std_joy);

// Read joystick 0
joy = joy_read(0);

// Now you can use any of the Macros to check the status
// These are the generic Macros in joystick.h
if (JOY_LEFT(joy)) ...
if (JOY_RIGHT(joy)) ...
if (JOY_BTN_1(joy)) ...

// cx16.h has its own more SNES specific Macros
// but they are still limited to 4 directions/4 buttons
// and map to the same buttons as the ones in joystick.h
// Button 3/4 and Select/Start are the same, Button A is Button 1, etc.
if (JOY_SELECT(joy))...
if (JOY_START(joy))...
if (JOY_BTN_A(joy))...
```

### Joystick Example
Take a look at `joy.c` for an example of using the Kernal Function to check joystick status. To build/run it, `make joy` and `make runjoy`. If you prefer to use `joystick.h`, just follow the example above, or take a look at the example in the Game Loop/Wait chapter.

>NOTE: If you look at the "wait.c" file, you will see a 2nd function `waitCount(unsigned short count)`. This is just a helper function that calls `wait()` a certain number of times. Its helpful if you want to wait for more than 1 cycle. `waitCount(60)` for instance would wait for 1 second.

## Mouse
CX16 has mouse support. The mouse pointer is NOT visible when the CX16 starts but it doesn't have to be visible for you to use the mouse. To change the mouse visibility and to check the position and button status of the mouse, the CX16 of course provides some Kernal Functions.

