#ifndef INPUT_H_H
#define INPUT_H_H

#include <stddef.h>

/* input parameters for the program */
struct parameters {
    /* matrix size: N rows, M columns */
    size_t N, M;

    /* maximum number of iterations */
    size_t maxiter;

    /* number of iterations for the periodic reduction */
    size_t period;

    /* number of threads */
    size_t nthreads;
};
 
void read_parameters(struct parameters* p, int argc, char **argv);

#endif
