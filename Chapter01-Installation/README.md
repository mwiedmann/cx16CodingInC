# Installation
To program in C on the CX16 you need a few things to get started:
- A C compiler that has a CX16 "target"
- An IDE (Integrated Development Environment) or text editor
- The CX16 Emulator
- (optional) `make` if on Windows

## C Compiler
There are several C compilers out there that work with the CX16 but we will be using the `cc65` compiler
- [cc65 main page](https://cc65.github.io/)
- [Source code and download](https://github.com/cc65/cc65)
- [CX16 docs for cc65](https://cc65.github.io/doc/cx16.html)

According to the cc65 site:
```
cc65 is a complete cross development package for 65(C)02 systems, including a powerful macro assembler, a C compiler, linker, librarian and several other tools.
```
Essentially, cc65 allows you to write C code and "target" the compilation to a bunch of different 65(C)02 systems. cc65 understands some of the basics of these systems and "knows" how to compile your code in a format that can be run on these systems. It also includes most of the C standard library functions for use on these systems (stdio, stdlib, etc.) so you can use class C functions like `printf`.

### Installing cc65
Visit the [Source code and download page](https://github.com/cc65/cc65) and use one of the Downloads OR clone the repo and run `make`. I had to do the `make` method on my Mac and it worked no problem. I'm not an expert on this part so I will have to direct you elsewhere (CX16 forums or Discord, or just google search) if you run into issues.

## IDE or Text Editor
Any editor that has basic C syntax support will be fine here. Choose your favorite. We will be running and compiling from the command line so don't worry too much about integrated support. I'm using `VSCode`. Its free, has some C extensions for basic support, and has an integrated terminal which is helpful.

## CX16 Emulator
The CX16 emulator, docs, rom, and other software is all available on the X16Community github.
- [Main Community Github](https://github.com/X16Community)
- [Emulator](https://github.com/X16Community/x16-emulator)
- [Latest Emulator Download (r42)](https://github.com/X16Community/x16-emulator/releases)

Download the r42 or r41 version of the emulator.

## Optional - Installing Make on Windows
If you are on a Mac or Linux system, you should already have `make` installed. On windows you may need to install make. I'm going to pass the buck to Google on this should you need/want to install make. This is optional because you can also run the build command manually or in a .BAT file/shell script.
