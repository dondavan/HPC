#ifndef OUTPUT_H
#define OUTPUT_H

#include "input.h"
#include <stddef.h>
#include <stdlib.h>

/* Reporting  */
struct results {
    size_t niter;   /* Number of Iterations */
    size_t row;     /* Board Row Height */
    size_t col;     /* Board Col Width */
    uint8_t* board; /* Pointer to Board Array*/ 
};

void report_results(struct results *r);

#endif
