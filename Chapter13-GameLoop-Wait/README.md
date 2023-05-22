# Game Loop and Wait
The CX16 will execute your code as fast as it can. Your C code is compiled into machine code and the CX16 simply executes those instructions. The CX16 runs at 8 MHz and each instruction takes a certain number of cycles to execute. Some of them have fixed cycles and some of them depend on the parameters you give them. For games, we have some requirements though. We have sprites and animations that we want to move and cycle at predictable speeds/rates. For instance, we may want the player to move across the screen at about 60 pixels every seconds. How do you do that? With our code executing as fast as possible, how do we know how far to move the player sprite every time through the loop? Is there a way to have our game loop execute in a predictable amount of time?

## VSYNC, Collisions, and other Interrupts
The CX16 has some internal things that happen or get updated on a heartbeat:
- Tiles are displayed
- Sprites are displayed and sprite collisions detected
- Inputs like joysticks are checked

These all happen essentially in sync with the screen refresh. This is 60 hz (60 times a second). Hooking into this heartbeat is what we need. We will then know that:
- Everything has been drawn to the screen
- Sprites are in their correct position
- All "interrupts" have been fired
- Sprite collision bits have been updated
- Joystick and other inputs have been updated
- IMPORTANT: A predictable amount of time has past

## How
The CX16 has an internal clock that we can read to use as a heartbeat. It is synchronized with the various interrupts and screen refresh. If you are an expert on the CX16 hardware/system, you are probably screaming that I'm glossing over and not accurately describing how this works. For our purposes though, just know that this is a good method for timing our game loop.

What we will do is, at the end of our game loop, we will look at the system clock and wait for it to tick. Once it ticks, we can continue with our next game loop. This will make our game run at a predictable rate of 60 hz. We can then move our sprites accordingly. For our sprite that we wanted to move at a speed of 60 pixels per second, we just have to move it 1 pixel each game loop. Your sprites can achieve slower speeds by moving every other game loop (or even less often), and you can achieve faster speeds by moving sprites multiple pixels each game loop. There are a variety of techniques you can use here. The important part though is that we now have a predictable heartbeat to time this with.

## Kernal Functions and RDTIM
Our `cc65` C compiler has a `time.h` library we can import that gives us a function to access the system clock. To read the system clock, it has to call a `Kernal Function`. These are built in system functions we can call to do a variety of low level operations. The CX16 [KERNAL](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md) doc has full details. Before jumping to the `time.h` function, let's briefly discus Kernal Functions and build our time code from scratch.

To call a Kernal Function, you execute instructions at special high memory locations starting at `0xFF00`. To do this, we will use the C `asm` operator to directly run some assembly language. Kernal Functions can take in parameters and generally leave their results in one or more processor registers. We will dive into Kernal Functions in more detail in a later chapter.

The function we are interested in is the `RDTIM (Read Timer)` function at memory address `$FFDE` (assembly language uses `$` for hex). When we call this function, it will put the current time into a few processor registers. We are interested in the value that goes in the `Accumulator (A) register` which will update on that 60hz heartbeat we want. Let's take a look at our `wait()` function:

```C
// Global vars so we have fixed memory locations for the `asm` code
unsigned char start, next;

void wait() {
    // jsr = Jump to Subroutine
    // This is the memory location of the RDTIM (Read Timer) Kernal Function
    asm("jsr $FFDE");

    // sta = Store Accumulator
    // This copies the value from the A register to a memory location
    // In this case its the memory location of our "start" variable
    asm("sta %v", start);

    // Now that we have the start time
    // Get the "next" time and keep looping until it changes
    do {
        asm("jsr $FFDE");
        asm("sta %v", next);
    }
    while (start == next);
}
```

## Cross Compilers and time.h
As I said earlier, we don't need to call this Kernal Function directly like this because `cc65` has already done this for us. `cc65` is a "Cross Compiler". This means that it provides libraries of functions for you (like time.h) that work across many systems. `cc65` supports a variety of systems that use 6502 processors.

### Quick Thoughts on cc65
If you only use the standard libraries that cc65 provides, there is a good chance that your code will run on all or many of the systems that cc65 supports. It also helps on the CX16 because it is a new system and things are still changing. If the location of a Kernal Function changes, the cc65 team can just update their library code and your program never has to change. It is good to learn how the CX16 and its Kernal Functions work before using the cc65 provided library though, and be aware that cc65 also does not provide EVERYTHING you need.

### Argument Against
There are a few arguments against using the cc65 libraries that are worth considering:
- The graphics and sound hardware on these systems are all pretty different, so some games are unlikely to cross-compile and work.
    - This is true but some programs WILL cross-compile and the libraries still abstract away a lot of complexity and make your programs easier to read.
- If you switch to a different compiler, the cc65 libraries you are referencing won't work.
    - This is true as well. There are other compilers out there that work for the CX16. If you switch to one of them, you may have to change any code that uses cc65 libraries. Although, you might be able to port those libraries over, and rewriting those parts wouldn't be too difficult.
- You won't learn how the underlying system works if you lean on cc65 to do it for you.
    - In these chapters you will learn how to do it manually first!

## A "time.h" Version of wait()
I looked at the code for `clock.s` which is the assembly language code for the `clock` function in `time.h`. I see a funny thing:

```assembly
jsr     RDTIM
```

This is the same Kernal Function we are calling in our manual wait function code. This makes sense. cc65 is just helping us out so we don't have to figure out how these Kernal Functions work. Let's refactor our `wait()` function to use `time.h`:

```C
#include <time.h>

// clock_t is just a typedef for an unsigned long
clock_t start, next;

void wait() {
    // Get the current time
    start = clock();

    // Keep checking the current time until it changes
    do {
        next = clock();
    }
    while (start == next);
}
```

You can see that the code is basically the same except that we use the `clock` function provided by `time.h` instead of calling the Kernal Function directly with `asm` code.

## Using wait()
Since this function will be used in most of your programs moving forward, we will start putting this code in its own `wait.c & wait.h` files and reusing it. If you look at the `makefile`, you will see we are including `wait.c` in the list of files to be compiled. Doing this and adding a `#include "wait.h"` in your programs lets you call `wait();`

## Examples
We have 2 examples to look at:
- `heartbeat.c` - This prints out a number every 60 heartbeats. Because this is 60hz, it should print 1 per second. You can use the stopwatch on your phone to verify this. It will print once per second.
    - To build/run this: `make hb`, `make runhb`
- `movechar.c` - This is our first "game like" program and makes use of `wait()`. You use the left/right cursor keys (or joystick) to draw `@` characters around (and off) the screen (it uses some code from the MapBase chapter). If you comment out the `wait()` statement, you will see that the game runs WAY too fast.
    - To build/run this: `make`, `make run`

We will dive more into the joystick code later, but hopefully this sneak peek is easy to follow.