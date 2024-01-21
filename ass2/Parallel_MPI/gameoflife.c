#include <time.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "gameoflife.h"
#include "fail.h"


#define     ALIVE   1
#define     DEAD    0
#define     ALIGN   32

static void cleanup(void)
{
    
}


void simulate(const struct parameters *p,struct results *r)
{

    /**************************************************/
    /*              Setting Up Game Board             */
    /**************************************************/
    /* Allocate Memory  */
    const size_t row = p->N;
    const size_t col = p->M;
    uint8_t(*restrict cur) = malloc(row * col * sizeof(uint8_t));
    uint8_t(*restrict old) = malloc(row * col * sizeof(uint8_t));
    if(cur == NULL || old == NULL)die("Malloc Failed");    /* Check if malloc succeed */

    /* Initialize Board */
    for(size_t i = 0; i < row; i++){
        for(size_t j = 0; j < col; j++){
            cur[i*col + j]    = DEAD;
            old[i*col + j]    = DEAD;
        }
    }
    /* Read-in Initial Board*/
    read_board("input/beehive.txt", row, col, old);
    /* Output Board for Report*/
    {
        r->niter    = 0;
        r->row      = p->N;
        r->col      = p->M;
        r->board    = old;
        report_results(r);
    }


    /**************************************************/
    /*                MPI Initialisation              */
    /**************************************************/
    int MPI_rank, MPI_world_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_world_size);
    
    /* Distribute Data Through Row Distribution */
    size_t chuck_size;
    size_t row_start, row_end, col_start, col_end;
    chuck_size = row / MPI_world_size;

    /* Assign Cell Bounday for Each Node */
    row_start = MPI_rank * chuck_size + 1;      /* Tight Boundary */
    row_end   = row_start + chuck_size - 1;     /* Tight Boundary */
    col_start   = 1; col_end     = col-1;       /* Border with permant DEAD cell, so we don't iterate over them*/
    if(MPI_rank == 0)row_start=1;                   /* Border with permant DEAD cell, so we don't iterate over them*/
    if(MPI_rank == MPI_world_size-1)row_end=row-1;  /* Border with permant DEAD cell, so we don't iterate over them*/

    printf("My rank: %d, Row start: %d, Row end:%d \n",MPI_rank,row_start,row_end);

    /**************************************************/
    /*                  Run Iteration                 */
    /**************************************************/
    size_t iter, i_row, j_col;
    size_t num_alive_neighbour;
    
    for(iter = 0; iter < p->maxiter; iter ++){

        /* Iterate Over Cells */
        for(i_row = row_start; i_row < row_end; i_row++){
            for(j_col = col_start; j_col < col_end; j_col++){

                num_alive_neighbour = 0;
                /* Count Alive Neighbours Around Current Cell */
                num_alive_neighbour = old[(i_row-1)*col + j_col-1] + old[(i_row-1)*col + j_col] + old[(i_row-1)*col + j_col+1] + 
                                      old[(i_row)*col + j_col-1]   +                            + old[(i_row)*col + j_col+1]   +
                                      old[(i_row+1)*col + j_col-1] + old[(i_row+1)*col + j_col] + old[(i_row+1)*col + j_col+1] ;

                /* Apply Rules */
                /* 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation. */
                if(num_alive_neighbour < 2){
                    cur[i_row*col + j_col] = DEAD;
                }
                /* 2. Any live cell with two or three live neighbours lives on to the next generation.  */
                if(old[i_row*col + j_col] == ALIVE && (num_alive_neighbour == 2 || num_alive_neighbour == 3)){
                    cur[i_row*col + j_col] = ALIVE;
                }
                /* 3. Any live cell with more than three live neighbours dies, as if by overpopulation. */
                if(num_alive_neighbour > 3){
                    cur[i_row*col + j_col] = DEAD;
                }
                /* 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction. */
                if(old[i_row*col + j_col] == DEAD && num_alive_neighbour == 3){
                    cur[i_row*col + j_col] = ALIVE;
                }

            }
        }

        /* swap old and cur board */
        {
            void *tmp = cur;
            cur = old;
            old = tmp;
        }

    }

    /* Output Board for Report*/
    {
        r->niter    = iter;
        r->row      = p->N;
        r->col      = p->M;
        r->board    = old;
        report_results(r);
    }


    /**************************************************/
    /*                   Cleaning Up                  */
    /**************************************************/
    free(old);
    free(cur);
    atexit(cleanup);
}
