# Controls and Input
You wouldn't have much of a game without a way for players to control things. The CX16 has a mouse, supports SNES style joypads, and of course has a full keyboard for input as well. The standard C library functions for reading keyboard input all work, but those are more appropriate for console, STDIN/OUT operations, and text based programs. For faster, lower level keyboard input we have another Kernal Function we can use. Mouse and Joystick input have their own Kernal Functions and also are well supported by cc65 libraries.

## Joystick
We had a quick peek at the cc65's joystick support in a previous chapter, but let's build it from scratch. There is a Kernal Function `joystick_get $FF56` that we can use to check the state of any connected joysticks. See the [Kernal](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md#function-name-joystick_get) docs for full details.

>NOTE: You don't need an actual joystick. The CX16 emulator has the cursor keys mapped to joystick direction, and z/x/a/s/d/c mapped to joystick buttons.

We pass it the joystick number (starting at 0) that we want to check. It will return 3 bytes worth of info about the yes(0)/no(1) (being pressed) state of all the buttons and pads. The 3rd byte says whether or not the joystick is present at all:

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
// Set the A Register to check joystick 0
asm("lda #0");

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
CX16 has mouse support. The mouse is NOT enabled when the CX16 starts. To enable the mouse and check the position and button statuses, the CX16 of course provides some Kernal Functions.

`mouse_config $FF68` - Enable/disable the mouse and set it's resolution. The resolution defines the region of the screen the mouse is restricted to. We will see how you can use this in tandem with the `screen_mode $FF5F` Kernal Function to give the mouse access to the entire screen.

You can review the [Kernal docs for mouse_config](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md#function-name-mouse_config) for more details, but you really just need 3 lines of `asm`:

```C
asm("jsr $FF5F");

// Load register A with 1 to enable the mouse pointer
asm("lda #1");

// Call mouse_config Kernal Function
asm("jsr $FF68");
```

`mouse_get $FF6B` - Get the x/y position and buttons statuses of the mouse.

The [Kernal docs for mouse_get](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md#function-name-mouse_get) will show you that there is a little more to do with this function. It returns data to you in 2 ways:

1. The A register will hold the mouse button statuses. In this example we are using a `static` var to store (`sta`) the A register in at the end.
2. Before we call mouse_get, we need to load the X register with a `Zero Page` memory address where it can store the X and Y positions (each 2 bytes, 4 total bytes). `ldx #2` sets it to address `2`, and we have 2 pointers (mouseX/Y) pointing at address 2 and 4 so we can check those values after.

Zero Page is just the first "page" of memory starting at address 0. On the CX16 its the first 256 bytes and some of it is reserved for special use. The [Zero Page docs](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2007%20-%20Memory%20Map.md#zero-page) will show you the reserved sections. Kernal Functions will store values in the `$0002-$0021	16 bit registers r0-r15 for KERNAL API` range, so we have to use those memory address. For this example we are using address `2`.

```C
// You can use global or static var for this
static unsigned char mouseButtons;

// Use 2 pointers to get the mouse data returned
// from the mouse_get Kernal Function
unsigned short *mouseX = (unsigned short *)2;
unsigned short *mouseY = (unsigned short *)4;

// Register X holds the zero-page memory address where
// mouse_get will store 4 bytes of mouse x,y data
// bytes 0-1=X position, bytes 2-3=Y position
asm("ldx #2");

// Call the mouse_get Kernal Function
asm("jsr $FF6B");

// Register A holds the button statuses. Get it.
asm("sta %v", mouseButtons);

// *mouseX and *mouseY now hold the x/y position of the mouse
```

## Mouse Pointer Graphic
When you enable the mouse, the Kernal makes use of `Sprite 0` to hold the mouse pointer image. This means we can change it! If you want mouse support but don't want a visible pointer, you can change the image to be all transparent color. Otherwise, you can change the pointer to be whatever you want. By default the pointer is a 16x16 pixel image in 256 color (8 bpp) mode.

In my tests, it seems to put the image at VRAM address `0x13000` but I'm not sure if this is fixed so I wouldn't rely on it. Instead, you can look at the first two bytes of Sprite 0 to see what the address is, then change the bytes at that address or point it to a new address where your new pointer image is located. Our example code get the address and modifies the pointer image so you can see how it is done.

## Mouse Example
Look at `mouse.c` for an example of all of this. It does the following:
- Enables the mouse
- Prints out the VRAM address of the mouse pointer image
- Prints the X/Y position and button statuses every second
- If you hold the mouse button, it will change the pointer to be an image of random colors.

To build/run it, `make mouse` and `make runmouse`

## Keyboard
You have the standard array of C `conio.h` console functions for getting keyboard input, but if you want a faster, non-blocking, lower level check to see if a key is pressed, you can use the Kernal Function `GETIN $FFE4`. It returns the PETSCII code of the last key pressed. Useful in your games if you need some extra (non-joystick) controls. See `joy.c` for this example. Build/run: `make key`, `make runkey`.

```C
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
```
