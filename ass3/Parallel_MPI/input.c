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
           "  -f FILE    Read initial board from FILE.\n"
           "  -x NUM     Initial pattern x coordinate on board.\n"
           "  -y NUM     Initial pattern y coordinate on board.\n"
           "  -k NUM     Set the iteration to report.\n"
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
    p->X = 1500;
    p->Y = 1500;
    p->maxiter = 5000;
    p->period = 10;
    p->nthreads = 1;
    p->input_fname = "input/beehive.txt";

    while ((ch = getopt(argc, argv, "f:hH:i:k:m:M:n:N:p:")) != -1)
    {
        switch(ch) {
        case 'i': p->maxiter = strtol(optarg, 0, 10); break;
        case 'k': p->period = strtol(optarg, 0, 10); break;
        case 'f': p->input_fname = optarg; break;
        case 'm': case 'M': p->M = strtol(optarg, 0, 10); break;
        case 'n': case 'N': p->N = strtol(optarg, 0, 10); break;
        case 'x': case 'X': p->X = strtol(optarg, 0, 10); break;
        case 'y': case 'Y': p->Y = strtol(optarg, 0, 10); break;
        case 'p': p->nthreads = strtol(optarg, 0, 10); break;
        case 'h': default: usage(argv[0]);
        }
    }

    if (!p->N || !p->M) die("empty grid");
    if (p->X > p->M || p->Y > p->N) die("initial board out of bound");
    

}

/* Reading File Describing Initial Alive Cells */
void read_board(const char *fname, size_t height, size_t width, size_t start_row, size_t start_col, uint8_t * data)
{
    FILE *f;
    if (!(f = fopen(fname, "r"))) die("Reading Init Board File Failed..");

    unsigned ncells, row, col;
    unsigned offset_row, offset_col;
    size_t i;

    /* Reading Amount of Alive Cells*/
    if (fscanf(f, "%u", &ncells) != 1) die("Invalid number of cells");

    /* Reading X,Y Axis of Cell*/
    for (i = 0; i < ncells; ++i)
    {
        if (fscanf(f, "%u", &row) != 1 || fscanf(f, "%u", &col) != 1) die("Invalid Input");

        offset_row = row + start_row;
        offset_col = col + start_col;

        if ( offset_row<1 || offset_row>height-1 || offset_col<1 || offset_col>width-1 ) die("Invalid Cell Position");


        data[offset_row*width + offset_col] = Alive;
    }
    

}

