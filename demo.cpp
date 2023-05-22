#include <stdio.h>
#include <stdlib.h>

int main () {

    volatile char lol[2000] = "";
    volatile int ok = 0;

    while (scanf ("%s", lol) != EOF){

        ok += 14;
    }
}