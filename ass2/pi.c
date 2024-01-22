#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{

    //initialize variables
    int i;
    double pi = 0;
    int niter = 1000000000;

    // Get timing
    double start,end;
    omp_set_num_threads(8);

    #pragma omp parallel
    {
        printf("Hello from process: %d\n", omp_get_thread_num());
    }

    start=omp_get_wtime();
    // Calculate PI using Leibnitz sum
    /* Fork a team of threads */
    int partial_Sum, total_Sum;

    #pragma omp parallel private(partial_Sum) shared(total_Sum)
    {
        partial_Sum = 0;
        total_Sum = 0;

        #pragma omp for
        {
            for(int i = 1; i <= 1000; i++){
                partial_Sum += i;
            }
        }
    }
    
    // Stop timing
    end=omp_get_wtime();

    // Print result
    printf("Pi estimate: %.20f, obtained in %f seconds\n", pi, end-start);

}