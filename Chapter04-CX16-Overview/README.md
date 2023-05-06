# CX16 and VERA Overview
Before we get into more programming, let's look at an overview of some CX16 and VERA features. The goal here is to just have a high level understanding of these or at least be aware of them. We will get deep into the specifics of each of these topics later so don't worry if some of the topics (like banked memory) don't click yet.

## CX16 Memory
The CX16 has 40KB of RAM, plus either 512KB or 2MB of "banked" RAM. Your program, its stack, its data, and any dynamically allocated memory all sit in that 40KB of "main" memory. The beginning part of the 40KB though is used for calling kernal functions, selecting banked memory, and other internal things. This eats up about 1KB.

### CX16 Banked Memory
The 512KB or 2MB of RAM is broken up into 8KB "banks". Only 1 bank can be active at a time. Putting a value in memory location 0x0 will select that as the "active" bank and map its 8KB of data to memory locations 0xA000-0xBFFF. So you have this dance of selecting the bank you need, read/write to it, switch to another 8KB bank, work that that bank, repeat. With 512KB you have banks 1-63 to play with. With 2MB you have banks 1-255 (bank 0 is reserved for kernal use). Bank switching adds some complexity to your code, BUT, some kernal/library functions help you. For instance, there is a function to load data from a file into banked RAM that handles rolling to the next bank when the 8KB is used up. This allows you to load files much larger than 8KB and not have to write additional code.

### Additional Special Memory Locations
After the first 40KB of memory, there are 256 bytes that are mapped to VERA registers and other I/O functions. These are the bytes we will interact with to program the VERA chip.

## VERA
VERA stands for (Versatile Embedded Retro Adapter). It is the video and audio processing chip for the CX16. Any graphics programming you do will go through the registers on this chip. Let's review some of its features before we try programming it.

## 640x480 resolution
The CX16 has a fixed resolution of 640x480. You can use lower resolutions by changing the "Active Display Horizontal and Vertical Scale" registers on the VERA (which we will discuss later), but the final image will always be automatically upscaled to 640x480 pixels for you when it is displayed on the screen.

## 2 Layers, both supporting either tile or bitmap mode
Think of "Layers" as pieces of transparent film that you can draw on. Layer 1 is on top of Layer 0, so anything drawn on Layer 1 will cover that same area on Layer 0. They generally serve as foreground and background layers in games. When we get into sprites, you'll see that each sprite can be displayed in front of the layers, between them, or behind them. This allows you to create some cool effects where the sprite is in front of your background graphics but behind the foreground. This gives the illusion of depth.

### Bitmap Mode
A layer that is in Bitmap Mode has a "pixel-to-pixel" representation in Video Memory. Meaning, for each pixel you want to be displayed on that layer, you set a value in the corresponding Video Memory location. This gives you full control over the graphics on that layer, BUT, this can use a LOT more memory.

### Tile Mode
In Tile Mode, you start with "tiles" loaded in Video Memory. These are a series of 8 or 16 pixel width/height images. You then tell the layer what tiles to display and where. This allows you to reuse tiles, flip them around, change their color palette, and you only need 2 bytes for each tile location so it is much more memory efficient. Most games will use tile mode to layout their levels and backgrounds. If you think about a game like Mario Bros., every level is constructed from a small number of tiles.

## Up to 128 sprites
Sprites are individually controllable images that can be up to 64x64 pixels. Tiles are fixed to a location and are generally for foreground/background graphics, but sprites are used for things like the player, missiles, lasers, enemies, and anything else that moves or animates. Sprites also have built in hardware collision detection. This is MUCH faster than having to check to see if anything collided each game loop. Sprites have a memory location pointing to their current image which you can change to cycle through animation frames or change how the sprite looks.

## Video RAM of 128KB
All of your tiles and tile layer info, bitmaps, and sprites have to fit into 128KB of Video RAM. This is a good amount for an 8 bit system but you still have to keep track of how you are using it so you don't run out. If you are using bitmap mode you have to be especially careful because 640x480 is over 300KB which is already more than the 128KB we have. You will generally use lower resolutions when using bitmap mode.

## Palette with 256 colors selected from a total range of 4096 colors
Each of the 256 colors in the palette is 2 bytes with 4 bits each for R, G, and B. That's 12 bits with the extra 4 bits unused. 12 bits gives you 4096 possible colors to choose from. Color 0 is transparent. The color info in your tiles and images will all use palette "indexes" rather than having RGB info (this is the purpose of a palette). An index is just a value of 0-255 (1 byte) that says what palette color to use. This saves a lot of memory and also allows you to change the palette on the fly and achieve interesting effects. This is called color cycling.

## Little Endian
**The CX16 is "Little Endian"**. Most of this system architecture decision is abstracted away and you don't have to worry about it, but it does come up so it's worth understanding. When you have a multibyte data type, let's say a 2 byte (16 bit) value, the system has to make a decision about how to store those 2 bytes in memory. It can either:
- Store the LOWER byte first (this is "Little Endian")
- Store the HIGHER byte first (this is "Big Endian")

Example - Let's say we have the 2 byte value `0x1234` stored at memory location `0x0`. This has the bytes `0x12` (the "higher" part of the number) and `0x34` (the "lower" part of the number). Here is how this can be stored in memory:
| Memory Addr -> | 0x0 | 0x1 |
| --- | --- | --- |
| Little Endian | 0x34 | 0x12 |
| Big Endian | 0x12 | 0x34 |

C compilers for the CX16 (and any system) are aware of this and handle everything for you. So when you are doing things like bit shifting using `>>`, you don't have to worry about Big vs Little Endian. It will work the same on any system. `>>` for instance will shift towards the lower part of the number and `<<` towards the higher part.

### When does it come up then?
If you have a 16 bit (or larger) value and you are setting or reading the individual bytes, you need to be aware of this. This can come up if you are loading bytes from a file or putting bytes into memory from an array and want to map some of those bytes to 16 bit numbers. You would need to have the bytes in the correct order. 

Another area, that we will get into in the next chapter, is when setting VRAM addresses in VERA. You will see that the VERA registers start with the "Lo" byte. This is so that you can copy a 16 bit number into them and the Little Endian order will be maintained. So even here you don't have to think about it too much because setting that register from a 16 bit value just works. Everything is already in the correct order, but if you notice that the "Lo" register is first you might wonder why (and now you know!).

# No homework, just some thoughts
There isn't really any coding you need to do for this chapter but I offer you a "kudos" and a pat on the back for taking this journey. You are probably getting a sense by now that coding on the CX16 is a bit different. If you've done any game programming on modern systems, it can feel strange to now have to worry about things like bank switching, color palettes, and limited video RAM. What I can say though is that it feels very rewarding to do this type of coding, especially when you see it working. Being this close to the hardware reminds us what is still underneath and abstracted away in all the modern systems we use. There will always be a place for this type of coding and systems like the CX16.