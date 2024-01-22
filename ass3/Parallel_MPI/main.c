#include "gameoflife.h"

int main(int argc, char **argv)
{
    struct parameters p;
    struct results    r;

    read_parameters(&p, argc, argv);

    simulate(&p,&r);

    return 0;
}