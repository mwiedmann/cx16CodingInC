# Hexidecimal vs. Binary vs. Decimal Numbers
Before we dive into some actual graphics programming on the CX16, let's take a moment to make sure you are comfortable with some topics around numbers and how to represent and manipulate them in C.

## Programming The VERA Video Chip
To do anything graphics related on the CX16, you will will be interfacing with the VERA video chip. The CX16 documentation has a lot of information about the VERA that is worth reading through, but in these tutorials we will go through it piece-by-piece and try to keep it as understandable as possible. To program the VERA, we will be dealing with individual bits and bit manipulation, memory locations and weird addressing modes, memory banks, video memory access using a single register, and other strange things. All of this requires us to use numbers to represent the memory locations and values going in-and-out of these locations. To do this we will use a combination of hexidecimal, binary, and (normal) decimal numbers.

## Different Ways to Represent Bits and Bytes
If you aren't familiar with hexidecimal numbers (henceforth referred to as "hex") or binary numbers, you may want to find an article or video to explain the basics (I only have a very brief explanation below). We will use hex when referring to memory locations because its shorter (and because the VERA docs use it too). We will use binary when dealing with setting individual bits because its easier to see what bits we are manipulating. You don't have to be a master or be able to convert decimal numbers to hex or binary in your head, just some basic understanding is fine.

### CX16 Hex Basics
Hex numbers are "base-16" and use the digits 0-9 and letters A-F. They look like `$9F20` in the CX16 documents. The `$` is used in BASIC programs in front hex numbers. In C, hex numbers have a leading `0x` and look like `0x9F20`. In this example, we have a 16 bit number. This is 2 bytes, also called 1 "word". 1 byte = 8 bits. 1 byte in hex only need 2 characters (besides the leading `$` or `0x`) and goes from 00 to FF (0 to 255). Examples in C: `0xFF` = 255, `0xA0` = 160. When you go to 2 bytes, you just add another 2 characters `0x9F20` = 40736.

### CX16 Binary Basics
You won't see binary numbers in the CX16 docs very often except when they refer to the bits in the VERA registers (more on that later). You will see `Bit 7 | Bit 6 | Bit 5 | Bit 4 | etc...` which refers to the 8 individual bits in that 1 byte memory location.

>A "register" is just a 1 byte (8 bit) memory location. It may be something special on the actual/physical CX16 hardware, but our code only sees registers as memory locations that have some special meaning.

In our C code we will sometimes use binary numbers when we need to set specific bits. Bits go from bit 7 to bit 0 (left to right), with bits on the left being the "bigger" values than to the right (just like in regular decimal numbers). In C, binary numbers have a leading `0b`. Examples: `0b1` = 1, `0b10` = 2, `0b10000000` = 128, `0b100000001` = 257 (which is more than 1 byte now).

## Bit Manipulation
To program the VERA, we will often need to turn on/off certain bits of a memory location. If we are setting the entire 8 bits, we can just set the value, but if we only want to turn on bit 5 for instance and leave the rest of the bits as they are, then we need to use some bit operators.

### Bit Operators
This tutorial isn't meant to be a full C programming course, so we will just review a few operations here. There are tons of great articles, videos, and courses available that cover this and other C topics.

Let's assume we have a variable `mem` that has some unknown value.

#### OR Operator
`You want to turn ON bit 4` (remember, bits go from 7-0, hi-to-lo)
```C
mem |= 0b10000;
```
Here we used the `OR` operator. This compares each bit in the number and sets it to 1 if it was already 1 OR if the new bit is a 1. In this case, all the new bits are 0 EXCEPT for bit 5 which is a 1. So all the existing bits will stay as they are (because OR with 0 won't change it) EXCEPT bit 5. Bit 5 will stay 1 if it was already 1 OR it will change to 1 since our new bit 5 is 1.

#### AND Operator
`You want to turn OFF bit 4`
```C
mem &= 0b11101111;
```
Here we used the `AND` operator. This compares each bit in the number and sets it to 1 ONLY IF BOTH the old bit AND the new bit are 1. In all other cases, the value of the bit will be set to 0. You can see we gave it all 1s EXCEPT for bit 4 which we set to 0. For the 1s, this means the bit will stay a 1 if it was already a one, otherwise it will end up a 0. For bit 4, it will ALWAYS become a 0 because `? AND 0` will ALWAYS be 0.

#### Bit Shifting
This one will be a little hard to see why it's useful until we get into some of the weird memory addressing we have to do with the VERA but I want you to at least understand what it does.

`You want to shift (or "slide") bits to the left or right`
Meaning, let's say you have `mem = 0b00000001` and you want to slide that 1st bit to the left 3 places.
```C
mem << 3;
```
The value would now be `mem = 0b00001000`. It slides ALL bits to the left. You can also slide bits to the right.
```C
mem >> 3;
```
That 1 would now be back in its original position. `mem = 0b00000001`.

BE CAREFUL OF DROP OFF!
Bits that are shifted to the right BEYOND bit 0 will DISAPPEAR. For bits that are shifted to the left, it depends on the size of the data. For 1 byte (8 bit) data types, bits will disappear when shifted BEYOND bit 7. For 2 byte (16 bit) data types, you have all the way to bit 15 before you have to worry about bits dropping off. Usually this isn't a problem and often is what you want.

## Sample Program
Take a look at the `main.c` file to see some examples of hex and binary numbers in C. One thing to note is the `%u` format specifier. This one tells printf to display the number as an `unsigned decimal integer`. A quick google search of `printf` will show you all the options. Go ahead an run it to see the results.

```C
printf("This is 1 %u\n", 0x1);
```
