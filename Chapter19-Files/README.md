# Files
So far, we have created tiles and sprites directly in code. This can be fine for some projects, but you can quickly run out of memory if your program is bloated with huge arrays for all of your graphics. Remember, everything in your program will be loaded into memory, so any hardcoded arrays for defining sprites and tiles will eat up memory when you load your program. A better way to do it is to load graphics from separate files. You also may want to load non-image data like level layouts, high-score lists, and saved game files. Speaking of saved games, you may also want to write files. The CX16 has an SD card slot and can read and write files from this device. There are kernal "Channel I/O" functions that are compatible with and have their origin in the C64. Our cc65 compiler has a `cbm.h` that exposes these file functions for us. We can use them to load files into RAM (and VRAM) and write files back to the SD card.

## 2 Byte Header
C64 files traditionally have a 2 byte header (first 2 bytes in the file) that optionally specifies a memory location to load the file into. You can still make use of this but for our examples we will control the memory location in code. This means that our files will NOT have the 2 byte header and we need to specify this when loading our files. You will see the kernal function `SETLFS` has a parameter for this. I wanted to point this out because some files you find and want to use may have the 2 byte header on them. You can either modify the file and strip the 2 bytes off, OR, just use the correct `SETLFS` param to skip the header.

## Kernal File Functions
As of when this README was created, the documentation for the [Kernal File Functions](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md) was incomplete, but it may be because these functions are backwards compatible with the C64 and have been around for awhile. The developers may have assumed we are all familiar with how they work (but I wasn't at least). Here is what you need:
- `SETNAME 0xFFBD` - Simply sets the filename of the file to work with
- `SETLFS 0xFFBA` - Sets the "Logical File" info for the file. This includes the following params:
    - First param is the Logical File Number. `Use 0` if you are just loading the file. You can use other values to keep multiple files open
    - Second param is the device number. The SD Card on the CX16 is `8`. (TODO: Add info about other device numbers)
    - The last param is the Secondary Address:
        - 0 - File has the 2 byte header, but skip it
        - 1 - File has the 2 byte header, use it
        - 2 - File does NOT have the 2 byte header
- `LOAD 0xFFD5` - Load the file. The first param here is important because it allows you to directly load data into VRAM. This is useful for loading tiles and sprites.
    - First param means:
        - 0, load into system memory.
        - 1, perform a verify.
        - 2, load into VRAM, starting from 0x00000 + the specified starting address.
        - 3, load into VRAM, starting from 0x10000 + the specified starting address.
    - Second param is the 16 bit address to load the file into

## cbm.h
cc65 provides functions that call these Kernal Functions for us. They have the exact same params but save you from having to deal with `asm` calls. That's why we're using C right ;). You can see the documentation for [cbm.h](https://cc65.github.io/doc/funcref.html#ss2.11) for more details. We will use:
- `cbm_k_setnam`
- `cbm_k_setlfs`
- `cbm_k_load`

No surprise they have the same name as the Kernal Functions they are wrapping.

## Loading a Tile Example
I have provided a small file `FLAME.BIN` that is a 16x16 pixel, 256 color tile (although it just uses a few colors). The example code loads the tile into the tileBase in VRAM, then fills the entire screen with the flame tile. It has examples of doing this by calling the Kernal Functions directly with `asm`, and using the `cbm.h` functions. You will see by using `cbm.h` that its only 3 lines of code vs. many more using `asm`, but I wanted you to see both.

`make files` to build, and `make runfiles` to run it.