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
