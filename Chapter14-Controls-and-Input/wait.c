#include <time.h>

// clock_t is just a typedef for an unsigned long
clock_t start, next;

void wait() {
    // Get the current time
    start = clock();

    // Keep checking the current time until it changes
    do {
        next = clock();
    }
    while (start == next);
}

void waitCount(unsigned short count) {
    unsigned short i;

    for (i=0; i<count; i++) {
        wait();
    }
}
