#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int ch;
    //initialize variables
    int i;
    double pi = 0;
    int niter = 1000000000;
    while ((ch = getopt(argc, argv, "i:")) != -1)
    {
        switch(ch) {
        case 'i': niter = strtol(optarg, 0, 10); break;
        }
    }
    printf("Iteration: %d\n", niter);

    // Get timing
    double start,end;
    start=omp_get_wtime();

    // Calculate PI using Leibnitz sum
    /* Fork a team of threads */
    #pragma omp parallel for reduction(+ : pi)
    for(i = 0; i < niter; i++)
    {
        pi = pi + pow(-1, i) * (4 / (2*((double) i)+1));
    } /* Reduction operation is done. All threads join master thread and disband */

    // Stop timing
    end=omp_get_wtime();

    // Print result
    printf("Pi estimate: %.20f, obtained in %f seconds\n", pi, end-start);
}
