#include <stdio.h>
#include "wait.h"

void main() {
    unsigned char i,a;

    a = 1;
    while(1) {
        for (i=0; i<60; i++) {
            wait();
            
        }
        printf("%u\n", a);
        a++;
    }
}