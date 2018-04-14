#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main (void) {

    srand(time(NULL));
    while(1) {
        int x = rand();
        printf("%d\n", x%15);
    }
    return 0;
}
