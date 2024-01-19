#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "gameoflife.h"


#define     ALIVE   1
#define     DEAD    0


static void cleanup(void)
{
    
}

void simulate(const struct parameters *p){

    printf("bla");

    atexit(cleanup);
}
