#ifndef INPUT_H_H
#define INPUT_H_H

#include <stddef.h>
#include <stdint.h>

/* input parameters for the program */
struct parameters {
    /* matrix size: N rows, M columns */
    size_t N, M;

    /* maximum number of iterations */
    size_t maxiter;

    /* number of iterations for the periodic reduction */
    size_t period;

    /* input file name */
    char *input_fname;

    /* number of threads */
    size_t nthreads;
};
 
void read_parameters(struct parameters* p, int argc, char **argv);

void read_board(const char *fname, size_t height, size_t width, size_t start_row, size_t start_col,  uint8_t * data);

#endif
