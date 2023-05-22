# Commander X16 Tutorial and Programming it in C
The purpose of this repo is to be a tutorial/guide about the Commander X16 and programming it with C. The main part of this is more a general programming guide for the CX16 and how it functions, so even if you aren't going to code in C, you will still find the information here valuable. We will cover how the VERA chip works and how we interact with Video Memory. We will cover graphics modes, layers, tiles and tilemaps, and sprites. We will use banked memory, kernal functions, and C library functions created for the CX16 (some of which have their roots in the C64). Each chapter comes with some C programs to demonstrate the topics just learned.

## cc65 Cross Compiler
The C code in here uses the [cc65 compiler](https://cc65.github.io) and leverages some of its libraries. If you are using a different compiler, I always show how to build it from scratch before using any cc65 libraries, so you will still find value here and should be able to use a different compiler.

## Using These Tutorials
The best first step is to clone the repo so you can work with it locally. Start with Chapter01 to get everything installed. Chapter02 then will help you get things set up, write your first program, and make sure everything is configured and running. After that, you are off and running.

## What is the Commander X16?
The CX16 is a "Modern Retro" 8 bit computer created by David Murray, AKA "The 8-bit Guy". He runs a successful YouTube channel centered on vintage computing. He also dabbles in music, electric car culture, off-grid energy production, computer refurbishment and resale. His web site is https://www.the8bitguy.com/

See his [FAQ](https://cx16forum.github.io/faq.html) for a full explanation of why the CX16 was created, but here is a snippet:

>The Commander X16 is The 8-Bit Guy’s dream computer, designed to evoke the same fondness and nostalgia many of us had for 8-Bit computers, while retaining closeness to the hardware from a programming perspective, unlike the Raspberry Pi and others. But more than that, it is intended not only as an educational tool but to solve some of the issues of finding an 8-Bit system to tinker with today; namely ever-increasing costs, auction site price gouging/sniping, lack of replacement parts, and unreliability of 30-year old hardware.

A few links:
- The CX16 Forums: https://cx16forum.com/
- The CX16 Community Github. It has the emulator and docs: https://github.com/X16Community

