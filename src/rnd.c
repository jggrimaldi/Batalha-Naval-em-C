#include "rnd.h"
#include <stdlib.h>
#include <time.h>

void rnd_seed(void) {
    srand((unsigned) time(NULL));
}

int rnd_int(int min, int max) {
    if (max <= min) return min;
    int range = max - min + 1;
    return (rand() % range) + min;
}