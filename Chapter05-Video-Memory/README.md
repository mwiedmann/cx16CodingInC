# Video Memory Access
I hinted earlier that accessing video memory (VRAM) is not a direct operation. It is a 2 step process:

1. Using the VERA "Addr" registers, set the VRAM address you want to access.
2. Read from or write to the VERA DATA0 register. The value is either pulled from or written to the address you set in step 1.

Why do we have to do this? The reason is because the CX16 uses 16 bit memory addresses. This means we can only access 64KB of total memory. We learned that the main memory is 40KB + some special registers + 8KB banked RAM + some other stuff. Where is the 128KB of VRAM? There is no way to map that much extra memory into a 16 bit addressing scheme. Instead, the architects of the CX16 are forced to use some kind of indirection to access all of this additional VRAM. This may seem inconvienient but I think you'll see that it isn't all that bad, and VERA provides an interesting helper in the form of "Address Increment" that makes it much easier to use.

## VRAM Addresses
To access each byte in 128KB requires a 17 bit address. 1KB is actually 1024 bytes because everything is always a power of 2 in computing. 128KB is then 128*1024 = `131,072`. Grab a calculator and do 2^17 (2 to the power of 17) and you will get `131,072`. With 17 bits we can "point to" any of the 131,072 bytes. To get 17 bits, we will need to use 2 bytes (16 bits) + "borrow" 1 bit from a 3rd byte.

## VERA VRAM Registers
VERA has 3 registers for us to use to set the VRAM address. 2 of them are full bytes, and in the 3rd register we only need 1 bit. VERA also has 1 register for us to read/write the value of that VRAM location. Let's look at a few of these registers. The latest version of this is always found in the [VERA Programmer's Reference](https://github.com/X16Community/x16-docs/blob/master/VERA%20Programmer's%20Reference.md), but here is the section you need for this chapter:

<style>
table, th, td {
  border: 1px solid;
}
</style>

<table>
	<tbody>
    <tr>
		<th>Addr</th>
		<th>Name</th>
		<th>Bit&nbsp;7</th>
		<th>Bit&nbsp;6</th>
		<th>Bit&nbsp;5 </th>
		<th>Bit&nbsp;4</th>
		<th>Bit&nbsp;3 </th>
		<th>Bit&nbsp;2</th>
		<th>Bit&nbsp;1 </th>
		<th>Bit&nbsp;0</th>
	</tr>
	<tr>
		<td>0x9F20</td>
		<td>Addr Lo</td>
		<td colspan="8" align="center">VRAM address bits 7-0</td>
	</tr>
	<tr>
		<td>0x9F21</td>
		<td>Addr Mid</td>
		<td colspan="8" align="center">VRAM address bits 15-8</td>
	</tr>
	<tr>
		<td>0x9F22</td>
		<td>Addr Hi and Settings</td>
		<td colspan="4" align="center">Address Increment</td>
		<td colspan="1" align="center">DECR</td>
		<td colspan="2" align="center">-</td>
		<td colspan="1" align="center">VRAM Address bit 16</td>
	</tr>
	<tr>
		<td>0x9F23</td>
		<td>DATA0</td>
		<td colspan="8" align="center">VRAM Data port 0</td>
	</tr>
</tbody></table>

## DATA0
You can see at 0x9F23 a register called DATA0. This is the register that we use to get/set the value in VRAM at the address we set in the "Addr L/M/H". Writing to this register will put the value into VRAM, and reading from this register will get the value from VRAM.

## Address Increment and DECR
We will come back to these but I want to give a quick explanation because you probably see these in `0x9F22` and are wondering what they are. I said that VERA offers some help with this VRAM indirection, well these are part of it. By setting the "Address Increment" mode, VERA will automatically adjust the VRAM address after EVERY read or write. What this means is that you can set the VRAM address once, then repeatedly read/write DATA0 and VERA will change (increment) the address for you. If you wanted to read 100 bytes from VRAM, you set the address increment to 1, set the VRAM address, then just read DATA0 100 times. Its really helpful. We will dive into all its uses later. DECR controls if the address increments or decrements.

## Write to VRAM Example
Let's say we want to write a value into VRAM at address `0x1A077`. In decimal this is byte # `106615`. In binary this is `0b11010000001110111`. We need to put that address into the 3 registers. Let's separate that binary number out into the 3 parts:

`1 10100000 01110111`

**"Little Endian" sidebar**

Since the CX16 is Little Endian, this `long` value would actually be stored like this:
`01110111 10100000 00000001 00000000`. You will see in our C code that this works well with the "Addr Lo" and "Addr Mid" registers. The order is the same and allows us to use an `unsigned short` (2 byte) variable to deal with the Lo/Mid together.

To put this address into the VERA "Addr" registers:
- The `1` goes the "Addr Hi" register bit 0 (VRAM Address bit 16)
- The middle bits `10100000` (160 or 0xA0) go into "Addr Mid" register
- The lo bits `01110111` (119 or 0x77) go into the "Addr Lo" register

Once that is done you can write a value to the DATA0 register and it will go into VRAM address `0x1A077`.

If you had the Address Increment value set then VERA would automatically increment the address for you and you could continue to read/write through VRAM.

## How to Read/Write Memory Locations and VERA in C
cc65 has `cx16.h` which contains some helpers for using the VERA registers. It also has `peekpoke.h` which has some Macros for reading (peeking) from and writing (poking) to memory locations. We will make use of these eventually, but since this is all about learning we will use raw C pointers for now. This will help you get more comfortable with what is actually going on.

### C Pointers
A dreaded topic for sure that has put many programmers into a fetal position rocking back-and-forth on their bathroom floor! You will not fear them here! We will make simple use of them.

A pointer is a variable that "points" to a memory address. You can do 2 things with them:

1. You can change what address it is pointing to
2. You can get and set the value that is it pointing to

What we want to do is to have a pointer for each VERA register we are working with. This will allow us to read and write values to those registers. We use a pointer for this because we know that these registers are at specific memory locations, so we have the pointer "point" at that memory location, then we can read/write from it.

Example - Let's create a pointer for the DATA0 (`0x9F23`) register.
```C
unsigned char *data0 = (unsigned char *)0x9F23;

// To write to that register
*data0 = 123;

// To read from that register
someOtherVariable = *data0;
```

Pointers can also point to 2 byte and 4 byte values as well. This is helpful if you want to write larger values or deal with multiple registers combined. A perfect example are the AddrLo + AddrMid registers. They are next to each other in memory, so we can use a 2 byte data type to write to them both at the same time.

Example - A 2 byte AddrLo + AddrMid pointer
```C
unsigned short *addrLoMid = (unsigned short *)0x9F20;

// write a 2 byte address to those registers
*addrLoMid = 0xA077;
```

### Using Different Data Types
You have to be careful when assigning data from one variable to another if the data types are different. C will allow it, BUT, you might not get the value you are expecting. Copying from a smaller data type to a larger one is generally safe because there is no fear of data loss. Copying from a larger data type to a smaller one though can lead to bugs if you aren't careful. What happens is that the correct number of bytes are copied, but higher bytes will be chopped off. Take this example:

```C
void main() {
    unsigned char foo = 0; // 1 byte data type
    unsigned short bar = 513; // 2 byte data type

    foo = bar;

    // This prints "1" because only the 1st byte of bar is copied (which has a 1)
    // You lost data and C didn't care
    printf("%u\n", foo);
}
```

This can be helpful when parsing a larger data type into smaller ones. For instance, you may use an `unsigned long` to hold a 17 bit VRAM address, but need to split it into a 2 byte Lo/Mid register and a 1 byte Hi register.

```C
    unsigned short addrLoMid;
    unsigned char addrHi;
    
    // Use a long (4 bytes) to hold the full VRAM address
    unsigned long vramAddr = 0x1A077;

    // Pull out the Lo/Mid (1st and 2nd) bytes
    // Because addrLoMid is a 2 byte data type, it will just get 2 bytes
    addrLoMid = vramAddr;

    // Shift >> the Lo/Mid bytes out.
    // This leaves the Hi (3rd) byte for us to grab
    // Because addHi is a 1 byte data type, it will just get 1 byte
    *addrHi = vramAddr>>16;
```

For setting the actual registers in the example above we will use pointers, but this example illustrates how the data type size impacts how many bytes it copies.

## Code Example
Look at `main.c` for an example of:
- Parsing a 17 bit VRAM address into the Address Lo/Mid/Hi registers
- Setting the "Address Increment" to 1
- Writing 10 values to VRAM
- Reading them back in to prove that the increment works and that we wrote data to VRAM.
- Print the values

It will use all of the techniques in this chapter.
**WARNING** When using `printf` be aware that printing to the screen will change the VRAM address, so don't try to print in the middle of working with VRAM.

## cx16.h
People writing cx16 C code must have to make pointers to all of these VERA registers all the time right? Yes. So the cc65 people made a library you can import `cx16.h` that has all of the registers and a bunch of other helpers. Instead of individual pointers, they use a single `struct` called `VERA` with a property for each register.

```C
#include <cx16.h>

void main() {
    VERA.address = 0xA077;
    VERA.address_hi = 0x1;

    VERA.data0 = 123;
}
```

You can see the definition of the VERA struct and a bunch of other helpers in cx16.h [here on their github](https://github.com/cc65/cc65/blob/master/include/cx16.h). Look for `struct __vera` (which is later defined as `VERA`). The helpers include `vpeek` and `vpoke` for reading/writing VRAM. These are fine for single useage but they are MUCH slower when you are dealing with multiple values because they set the address every time. It is still best to use the data0/address increment method when you can.

I have included `main-cx16.c` that uses the `VERA` struct instead of individual pointers for the registers. You will find that we will still use pointers a lot in our code so don't think you've escaped them just yet!