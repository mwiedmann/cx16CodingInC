# Hello, World!
Before we dive into VERA programming, let's create a simple C program to make sure cc65 and your emulator are set up correctly. We will also create a `makefile` to compile and run your program. We will expand this makefile throughout the tutorials to compile the additional C files and libraries we make.

## Makefile or not?
The makefile is a text file that contains the recipe for building your program. They are common with C programs and make it easy for someone who just downloaded your software to build it. All they have to type is `make` and voila. There is PLENTY of info about makefiles out there and it is an interesting topic, so please Google it if you are interested.

Compiling our CX16 programs will not be a very complicated process, so you do NOT have to use a makefile. We will have a 1 line command to compile our code into a .PRG file, and a 1 line command to run our program in the emulator. The commands will be documented in each chapter. You are welcome to use a .BAT file on Windows, a shell script on Linux/Mac, or just run the commands manually.

## Tutorial Setup
I recommend cloning/downloading this repo for easy access to the tutorials in each chapter along side your own work, but this is NOT required.

If you ARE cloning the repo, then I recommend for each chapter that you create a folder called "ChapterXX-Me" (where XX is the chapter number) along side the chapter for you to work in.

If you are NOT cloning the repo, start by creating a folder to work in. For each chapter, create a "ChapterXX-Me" folder to work in. ALSO, create a folder "x16emu" for the emulator.

### Emulator Setup
In the folder you cloned or created, there should be a folder "x16emu". Once you have downloaded and unzipped the emulator, copy all of the files into this directory. The commands in the tutorials to run your programs all assume the emulator is in this directory.

## Chapter 2 Setup

### Create the folder
Create a folder "Chapter02-Me". The "Me" part just helps separate your (Me) work from any chapters that you may have copied from the repo.

### Create the makefile
Create a file called `makefile` with the following code:
```make
CC=cl65

make:
	$(CC) -O -o CH1.PRG -t cx16 main.c

run:
	../x16emu/x16emu -prg CH1.PRG -run
```
`NOTE: the ../ style pathing may need to be adjusted on Windows. You can always change this to be a full path to the x16emu executable.`

Once we have written some code, to build your program, you simply open a terminal in the Chapter 1 directory and run: `make`. It will create your .PRG progam and also leave a few `.o` object files behind.

To run your program you can run: `make run`.

If you are NOT using a makefile then you can simply run these commands manually, or as part of a batch/shell script. Just substitue `cl65` for the `$(CC)`:
```
cl65 -O -o CH2.PRG -t cx16 main.c
```

### What are these command options?
- `cl65` - This is the cc65 compile & link utility. It combines a few of the building steps (compiling, linking, etc.) into one smart utility. 
- `-O` - optimize. This optimizes your code in a variety of ways beyond the scope of this tutorial but I find it really helps performance to use this flag.
- `-o CH2.PRG` - output. This specifies the output file for your program as CH2.PRG.
- `-t cx16` - target. This tells cc65 to use the CX16 as the target system for this program. cc65 has a long list of supported systems that all use the 6502 family of processors.
- `main.c` - The last arguments are the list of all the .c files you want compiled as part of this program. There are flags that tell cc65 to include entire directories but most of our examples will just have a few files so we can just list them.

## Hello, World!
Create a file `main.c` in your `Chapter02-Me` folder with the following code:
```C
#include <stdio.h>

void main() {
    printf("Hello, World!\n");
}
```

### Compile and run it!
Open a terminal in your `Chapter02-Me` directory and run `make` (or manually run the build command as detailed above). This will create the `CH2.PRG` program file and a `main.o` "object file". We only care about the .PRG file and the .o file can be deleted if you wish since it is just part of the compilation/linking process.

To run your program in the emulator, run: `make run` (or manually run it as detailed above).

You should see the emulator start, load your program, and output `"Hello, World!" before exiting.

## Conclusion
You just ran a C program on the CX16! Congrats! Looks like you have everything set up correctly. If you know anything about C, you can see that we used `printf` which is part of the `stdio` C standard library. Most of the standard library is available for you to use, so without knowing anything about the CX16 or VERA programming, you can still run a wide variety of C programs on the CX16. We will use some of the standard library in our programs later on.
