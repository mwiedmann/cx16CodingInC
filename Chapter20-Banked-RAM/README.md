# Banked RAM
We briefly discussed this back in the CX16 Overview chapter, but now let's dive deeper into Banked RAM. By now you should be used to the indirection method we use to read/write VRAM using the VERA `address, address_hi, and data0` registers. We learned that the reason for this is that the CX16 has a 16 bit memory addressing scheme which only lets you access 64KB of RAM. With 128KB of VRAM, there is no way to access all of those addresses with just 16 bits. We have a similar problem with extended RAM, but its even worse. The CX16 can have up to 2MB of memory. Rather than using another indirection method, for extended memory the CX16 maps 8KB of extended RAM into the 16 bit address space which you can then use like regular RAM.

The 512KB or 2MB of RAM is broken up into 8KB "banks". Only 1 bank can be active at a time. Putting a value in memory location `0x0` will select that as the "active" bank and map its 8KB of data to memory locations `0xA000-0xBFFF`. So you have this dance of selecting the bank you need, read/write to it, switch to another 8KB bank, work that that bank, repeat. With 512KB you have banks 1-63 to play with. With 2MB you have banks 1-255 (bank 0 is always reserved for kernal use). Bank switching adds some complexity to your code, BUT, some kernal/library and cc65 functions help you. For instance, there is a function to load data from a file into banked RAM that handles rolling to the next bank when the 8KB is used up. This allows you to load files much larger than 8KB and not have to write additional code.

## Uses for Banked RAM
There are a few reasons to use banked RAM.
1. Hold sprite and tile image data and sound data - Your games may end up having lots of different sprites and tileset that are not always active. Meaning, you may have sprite images for your player that are only used when they have certain upgrades. There is no reason for them to eat up space in VRAM if you aren't using them, and you might not even have enough VRAM to do so. Instead, load them into Banked RAM and quickly copy them to VRAM when you need them. Same thing for your various tilesets (which can eat up A LOT of memory). The Kernal has a function to bulk copy memory this way that is nearly instant. This allows you to have tons of graphics available to your game without having it all in VRAM.
2. Holding extended data - If you have some larger data structures for your game that don't need to stay in main RAM the entire time, put it in Banked RAM and swap it in when you need it. Perhaps your game has many levels, and each time the player exits a level you want to save the state of the level so when they return later you know what monsters are still alive, which piles of treasure were already picked up, etc. This can eat up a lot of memory if you have many levels. Store it in Banked RAM and swap it in when you need to redraw that level. 
3. Running Code - Since the 8KB banks are accessed as if they are just part of normal RAM, they can actually hold code. This allows your programs to be significantly larger than the 30KB or so size limit (it varies). Running code this way though does have some limitations, extra steps, and requires some extra help from cc65 if you are writing in C (thanks to CX16 forum user `Manannan` for how to do this!)

## Helpful Functions Provided
If you have a number of bytes you need to copy somewhere else in RAM, the CX16 has a Kernal Function for you. The Kernal docs detail [memory_copy](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md#function-name-memory_copy). It copies a block of bytes from one memory location to another. It has special logic for copying data to VRAM. When using it to write to the `VERA.data0 0x9F23` register (or any VERA register), it knows to keep spamming the data into that register. This allows you to use it to copy the bytes to VRAM.

The Kernal `LOAD $FFD5` function and `cbm_k_load` (which just calls the Kernal Function), will automatically roll to the next RAM Bank if you are loading a file into banked RAM and that Bank runs out of space. This allows you to load large files starting in a RAM Bank and it will span as many banks as it needs to load the file automatically. If you then later want to copy those bytes out of banked RAM into VRAM, you need to call the `memory_copy` function for each RAM Bank (it won't handle the automatic rollover for you).

## Loading Tile Images Example
In this example, we have 2 tile images (FLAME.BIN and SNOW.BIN). They are 16x16 pixel, 256 color tiles. We want to demonstrate loading these images to banked RAM and copying them to VRAM. We do the following:
- Load each image into its own bank.
- Fill the entire screen with tile index 0 (which starts as just random junk).
- Inn a loop, use the `memory_copy` function to alternate between copying each one into VRAM. You will immediately see the screen fill with that tile.
- There is a 1 second delay between each copy so you can see the new tiles for a moment.

`make tiles` to build it. `make runtiles` to run it.

## Code in Banked RAM
Besides storing images and data in Banked RAM, you can also load code into a RAM Bank and execute it. This is helpful because once your program size starts to get around 30KB, you are in danger of running out of RAM. There isn't an exact size limit because it depends on how much memory your variables and other data take up. Banked RAM is mapped into the 16 bit address space starting at `0xA000`, so it can hold executable code just like the rest of RAM. When compiling code that should go into Banked RAM though, the compiling/linking process needs some help understanding this.

### #pragma code-name
>The ‘#pragma’ directive is the method specified by the C standard for providing additional information to the compiler, beyond what is conveyed in the language itself.

cc65 provides `#pragma code-name`. This pragma changes the name used for the CODE segment (the CODE segment is used to store executable code). The argument is a string enclosed in double quotes. For this pragma to work, you also have to include an extra configuration file in your `cl65` compilation paramaters of `-C cx16-bank.cfg`. This config file is included with cc65 and helps the compiler understand CX16 Banked RAM.

To use it, you wrap a number of functions with the pragma like this:
```C
#pragma code-name (push, "BANKRAM01")
void callBank1() {
    printf("This function runs in Bank 1\n");
}
#pragma code-name (pop)
```

`push` will push that CODE segment onto the stack and the compiler will use that CODE segment until you `pop` it off and it returns to the previous CODE segment.

The string param must be in the form `"BANKRAMXX"` where XX is the bank number you will load the code into.

When you compile this code, any code inside the `pragma` will be compiled into a separate file with the filename `<OUTPUT-FILE>.XX` where `<OUTPUT-FILE>` is the output file from your compile command (the `-o` compiler option), and the `XX` is the Bank number in hex.

### Banked Code Size
Some symbols will exist when you use `#pragma code-name` that hold the size of the code in each Bank. You can pull them in with an `extern` like this example:
```C
extern void _BANKRAM01_SIZE__[], _BANKRAM02_SIZE__[];
```
They have the pattern of `_BANKRAMXX_SIZE__` where XX is the Bank number in hex.

### Using Code Compiled into a Banked RAM File
You will now have your main code compiled into its usual program file, and any Banked RAM code compiled into its own file(s). So how do you use the Banked RAM code? Its pretty straightforward. You just have to load those files into their correct Banked RAM (using the same technique you learned in this chapter for loading tile images into Banked RAM). Then, when you want to call those functions, you have to first swap to the correct RAM Bank. The steps:
1. In your main program, load all of the Banked RAM code files into their correct Banks.
2. When you want to call one of those functions:
    - Switch to the correct Bank
    - Call the function
    - Optional: Switch back to the previous Bank. This is generally a good idea and but requires a few extra lines of code to store the current Bank before switching Banks, then restoring the original Bank after calling the function.

### Limitations
- The current config only supports Banks 1-15 (more info coming on why). You will see it always spits out 15 files with the extensions `01-0f`. The ones with code will contain the compiled code and the rest will be empty. Use the ones you need and feel free to delete the rest. This still gives you 15 Banks of code storage though which is 120KB.
- You can't call Banked code directly from another Bank. Meaning, if you are running a function in Bank 1, you can't DIRECTLY call a function in Bank 2. The call stack doesn't know how to handle this when it returns back to the original function. You have to use a "trampoline function" to do this. In `code.c` you will see an example of this. Inside `callBank1` (which runs from Bank 1), we call `foo` which is in "low" RAM. `foo` then calls `callBank2A` and `callBank2B` which runs on Bank 2. This is ok because we returns to "low" RAM before jumping to another Banked RAM function. You can create as many trampoline functions like this as you need.

### Banked Code Example
Look at `code.c` for an example of all of this. `make code` to build it, `make runcode` to run it. You will see the 15 Banked code files created but we only use 2 of them.