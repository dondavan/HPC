#include "output.h"
#include "fail.h"
#include <stdio.h>


/* Write Board Array to TXT file*/
void report_results(struct results *r)
{
    char filename[50];
    sprintf(filename, "output/iteration_%zu.txt", r->niter);

    FILE *f = fopen(filename, "w");
    if (f == NULL)die("Error opening output file!");
    
    /* Print Board Information */
    fprintf(f, "%zu %zu\n", r->row, r->col); 
    for(size_t row = 0; row < r->row; row++){
        for(size_t col = 0; col < r->col; col++){
            fprintf(f, "%u",(r->board)[row*r->col + col]); 
        }
        fprintf(f, "\n"); 
    }

    fclose(f);

}

