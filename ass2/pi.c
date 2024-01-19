#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


int main (int argc, char *argv[])
{
  int nthreads = 8;
  int ch;
  while ((ch = getopt(argc, argv, "p:")) != -1)
  {
    switch(ch) {
      case 'p': nthreads = strtol(optarg, 0, 10); break;
    }
  }

  omp_set_num_threads(nthreads);

  printf("Number of threads: %d\n",nthreads);

  //initialize variables
  int i;
  double pi = 0;
  double val = 0;
  int niter = 1000000000;

  // Get timing
  double start,end;
  start=omp_get_wtime();

  // Calculate PI using Leibnitz sum
  /* Fork a team of threads */
  #pragma omp parallel for private(val) reduction(+ : pi)
  for(i = 0; i < niter; i++)
  {
     val += pow(-1, i) * (4 / (2*((double) i)+1));
  } /* Reduction operation is done. All threads join master thread and disband */

  // Stop timing
  end=omp_get_wtime();

  // Print result
  printf("Pi estimate: %.20f, obtained in %f seconds\n", pi, end-start);
}