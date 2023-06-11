# Files
So far, we have created tiles and sprites directly in code. This can be fine for some projects, but you can quickly run out of memory if your program is bloated with huge arrays for all of your graphics. Remember, everything in your program will be loaded into memory, so any hardcoded arrays for defining sprites and tiles will eat up memory when you load your program. A better way to do it is to load graphics from separate files. You also may want to load non-image data like level layouts, high-score lists, and saved game files. Speaking of saved games, you may also want to write files. The CX16 has an SD card slot and can read and write files from this device. There are kernal "Channel I/O" functions that are compatible with and have their origin in the C64. Our cc65 compiler has a `cbm.h` that exposes these file functions for us. We can use them to load files into RAM (and VRAM) and write files back to the SD card.

## 2 Byte Header
C64 files (which CX16 files are based on) traditionally have a 2 byte header (first 2 bytes in the file) that optionally specifies a memory location to load the file into. You can still make use of this but for our examples we will control the memory location in code. This means that our files will either NOT have the 2 byte header or we will skip it. We need to specify this when loading our files. You will see the kernal function `SETLFS` has a parameter for this. I wanted to point this out because some files you find and want to use may have the 2 byte header on them. You can either modify the file and strip the 2 bytes off, OR, just use the correct `SETLFS` param to skip the header.

## Filenames
I have found the following to be true with filenames but I don't have official documentation around this:
- Filenames on the files themselves should be ALL UPPERCASE. Meaning something like: `HIGH.BIN`. When you use SAVE to save a file, you will see that it makes it UPPERCASE as well.
- Filenames in your CODE should be lowercase though. Meaning, to load `HIGH.BIN`, you code would look like `cbm_k_setnam("high.bin");`
- I've had mixed success loading files from subdirectories. It works in most cases but I've had people complain that it fails for them in other cases. I need more research here, but my recommendation for now is to just load/save files from the current directory. The official web emulator also doesn't (currently) allow loading files from a subdirectory, so if you want to use the "Try It" functionality on the official forums, you'll need to only load files from the same directory as the .PRG file.

## Kernal File Functions
As of when this README was created, the documentation for the [Kernal File Functions](https://github.com/X16Community/x16-docs/blob/master/X16%20Reference%20-%2004%20-%20KERNAL.md) was incomplete, but it may be because these functions are backwards compatible with the C64 and have been around for awhile. The developers may have assumed we are all familiar with how they work (but I wasn't at least). Here is what you need:
- `SETNAME 0xFFBD` - Simply sets the filename of the file to work with
- `SETLFS 0xFFBA` - Sets the "Logical File" info for the file. This includes the following params:
    - First param is the Logical File Number. `Use 0` if you are just loading/saving the file. You can use other values to keep multiple files open
    - Second param is the device number. The SD Card on the CX16 is `8`. (TODO: Add info about other device numbers)
    - The last param is the Secondary Address:
        - 0 - File has the 2 byte header, but skip it
        - 1 - File has the 2 byte header, use it
        - 2 - File does NOT have the 2 byte header
- `LOAD 0xFFD5` - Load the file. The first param here is important because it allows you to directly load data into VRAM. This is useful for loading tiles and sprites. `LOAD` returns you the last address it loaded a byte into. The difference between this address and the address you passed to `LOAD` (the "starting address"), is the number of bytes loaded.
    - First param means:
        - 0, load into system memory.
        - 1, perform a verify.
        - 2, load into VRAM, starting from 0x00000 + the specified starting address.
        - 3, load into VRAM, starting from 0x10000 + the specified starting address.
    - Second param is the 16 bit "starting address" to load the file into
- `SAVE 0xFFD8` - Save bytes from memory to a file. Save ALWAYS adds the 2 byte header. There is no avoiding it. If you really want to avoid this, you can use `BSAVE` instead, but cc65 doesn't have a C function for `BSAVE`. NOTE: If you want to overwrite an existing file you MUST prefix the filename with `@:`. Save takes 2 params:
    - Starting memory address
    - Ending memory address
- `BSAVE 0xFEBA` - Same as `SAVE` but doesn't add the 2 byte header. There is no C function for this.

## cbm.h
cc65 provides functions that call these Kernal Functions for us. They have the exact same params but save you from having to deal with `asm` calls. That's why we're using C right ;). You can see the documentation for [cbm.h](https://cc65.github.io/doc/funcref.html#ss2.11) for more details. We will use:
- `cbm_k_setnam`
- `cbm_k_setlfs`
- `cbm_k_load`
- `cmb_k_save`

No surprise they have the same name as the Kernal Functions they are wrapping.

## Loading a Tile Example
I have provided a small file `FLAME.BIN` that is a 16x16 pixel, 256 color tile (although it just uses a few colors). The example code loads the tile into the TileBase in VRAM, then fills the entire screen with the flame tile. It has examples of doing this by calling the Kernal Functions directly with `asm`, and using the `cbm.h` functions. You will see by using `cbm.h` that its only 3 lines of code vs. many more using `asm`, but I wanted you to see both.

`make tile` to build, and `make runtile` to run it.

## Reading Files into C Data Structures
Non-graphical data from files often needs to be loaded into data structures. You might have something like a high score list that has multiple records with a name and score. In your code you might have a `struct Score` defined for these records. `LOAD` helps you get this data into memory, but then you need to have the proper data structures pointing to this memory to use it. There are a few ways to read something like this into a C data structure.

### Easy Way (Uses a Little More Memory)
You can create an array of your `structs` with a fixed size that you know is large enough to hold the file. You can then read the data directly into the array. `LOAD` returns you the ending address loaded into so you can calculate the number of bytes loaded. You can use that to calculate how many records were loaded and hence how many "valid" records are in your array. This is what we do in the `score.c` example.

```C
typedef struct Score {
    unsigned char name[4];
    unsigned short score;
} Score;

// 100 records should be enough (but wastes memory)
Score scoreData[100];

// Need to track how many records we have
unsigned char scoreDataRecords;

// ....

unsigned short finalMemAddr, bytesRead;

cbm_k_setnam("high.bin");
cbm_k_setlfs(0, 8, 0); // File has the 2 byte header but skip it

// load returns the last memory address written to
finalMemAddr = cbm_k_load(0, (unsigned short)scoreData);

// Calculate the number of bytes read as the difference between
// the starting and ending addresses
bytesRead = finalMemAddr - (unsigned short)scoreData;

// Calculate the number of "Score" records we read as:
// The total bytes read / the sizeof a Score struct "sizeof(Score)" 
scoreDataRecords = bytesRead / sizeof(Score);
```

### Harder Way (More Memory Efficient)
Rather than a fixed size array which may waste memory. You can read the file into a temporary buffer (or even BANKED RAM). Using the ending address returned from `LOAD`, you can get the number of bytes loaded. You can then use `malloc` to allocate exactly how much memory you need. You then copy the bytes from where they were loaded into your newly allocated memory. You can use that struct pointer just like an array, but you've now only used exactly the memory needed. The downside here is if you want to add more records in memory. You then need to allocate more memory. This method works well if you are just loading and using the data.

## Loading and Saving a High Score File Example
I have included a file `HIGH.BIN` that contains a high score list:
```
MRW - 43789
HRP - 21413
JLM - 9841
```
The binary data in this file is 3 "Score" records. Each record is 6 bytes:
- 4 byte unsigned char array for the name/initials (includes 0/null terminator)
- 2 byte unsigned short for the score

`score.c` reads this file into an array of `Score` structs and prints the list. It then adds a new score to the file, reads it in again, and prints the new list. It gives a good example of reading a file into a data structure and writing out a data structure to a file. Remember that this file (and any file created with `SAVE/cbm_k_save`) will have the 2 byte header, so use the proper `SETLFS` param when reading it.

`make score` to build, and `make runscore` to run it. `make newhigh` to reset the HIGH.BIN file back to the original 3 scores (it just makes a copy of HIGH_ORG.BIN which is the original).