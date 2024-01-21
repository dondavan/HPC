#include "input.h"
#include "fail.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#define     Alive   1
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

    if (!p->N || !p->M) die("empty grid");

}

/* Reading File Describing Initial Alive Cells */
void read_board(const char *fname, size_t height, size_t width, uint8_t * data)
{
    FILE *f;
    if (!(f = fopen(fname, "r"))) die("Reading Init Board File Failed..");

    unsigned ncells, row, col;
    size_t i;

    /* Reading Amount of Alive Cells*/
    if (fscanf(f, "%u", &ncells) != 1) die("Invalid number of cells");

    /* Reading X,Y Axis of Cell*/
    for (i = 0; i < ncells; ++i)
    {
        if (fscanf(f, "%u", &row) != 1 || fscanf(f, "%u", &col) != 1) die("Invalid Input");
        if ( row<1 || row>height-1 || col<1 || col>width-1 ) die("Invalid Input");
        data[row*width + col] = Alive;
        //printf("%d %d %d\n",row,col,data[row*width + col]);
    }
    

}

