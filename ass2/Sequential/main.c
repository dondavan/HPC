#include "gameoflife.h"

int main(int argc, char **argv)
{
    struct parameters p;

    read_parameters(&p, argc, argv);

    simulate(&p);

    return 0;
}