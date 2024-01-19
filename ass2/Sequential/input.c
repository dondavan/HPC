#include "input.h"
#include "fail.h"
//#include "config.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>


static void usage(const char *pname)
{
    printf("Usage: %s [OPTION]...\n"
           "\n"
           "  -n NUM     Set cylinder height to ROWS.\n"
           "  -m NUM     Set cylinder width to COLUMNS.\n"
           "  -i NUM     Set the maximum number of iterations to NUM.\n"
           "  -k NUM     Set the reduction period to NUM.\n"
           "  -p NUM     Number of threads to use (when applicable).\n"
           "  -h         Print this help.\n"
           ,pname);
    exit(0);
}

void read_parameters(struct parameters* p, int argc, char **argv)
{
    int ch;

    /* set defaults */
    p->N = 3000;
    p->M = 3000;
    p->maxiter = 5000;
    p->period = 10;
    p->nthreads = 1;

    while ((ch = getopt(argc, argv, "hH:i:k:m:M:n:N:p:")) != -1)
    {
        switch(ch) {
        case 'i': p->maxiter = strtol(optarg, 0, 10); break;
        case 'k': p->period = strtol(optarg, 0, 10); break;
        case 'm': case 'M': p->M = strtol(optarg, 0, 10); break;
        case 'n': case 'N': p->N = strtol(optarg, 0, 10); break;
        case 'p': p->nthreads = strtol(optarg, 0, 10); break;
        case 'h': default: usage(argv[0]);
        }
    }

    printf("Parameters:\n"
           "  -n %zu # number of rows\n"
           "  -m %zu # number of columns\n"
           "  -i %zu # maximum number of iterations\n"
           "  -k %zu # reduction period\n"
           "  -p %zu # number of threads (if applicable)\n",
           p->N, p->M, p->maxiter, p->period,
           p->nthreads);

    if (!p->N || !p->M) die("empty grid");
\
}