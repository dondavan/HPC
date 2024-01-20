#include <time.h>
#include <math.h>
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

    printf("Parameters:\n"
    "  -n %zu # number of rows\n"
    "  -m %zu # number of columns\n"
    "  -i %zu # maximum number of iterations\n"
    "  -k %zu # reduction period\n"
    "  -p %zu # number of threads (if applicable)\n",
    p->N, p->M, p->maxiter, p->period,
    p->nthreads);

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
            cur[i + j]    = DEAD;
            old[i + j]    = DEAD;
        }
    }
    

    /**************************************************/
    /*                  Run Iteration                 */
    /**************************************************/
    size_t iter,i_row,j_col;
    size_t num_alive_neighbour;
    size_t row_start = 1;   size_t row_end  = row-1;    /* Border with permant DEAD cell, so we don't iterate over them*/
    size_t col_start = 1;   size_t col_end  = col-1;    /* Border with permant DEAD cell, so we don't iterate over them*/

    for(iter = 0; iter < p->maxiter; iter ++){

        /* Iterate Over Cells */
        for(i_row = row_start; i_row < row_end; i_row++){
            for(j_col = col_start; j_col < col_end; j_col++){

                num_alive_neighbour = 0;
                /* Count Alive Neighbours Around Current Cell */
                num_alive_neighbour = old[i_row-1 + j_col-1] + old[i_row-1 + j_col] + old[i_row-1 + j_col+1] + 
                                      old[i_row + j_col-1]   +                      + old[i_row + j_col+1]   +
                                      old[i_row+1 + j_col-1] + old[i_row+1 + j_col] + old[i_row+1 + j_col+1] ;

                /* Apply Rules */
                /* 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation. */
                if(num_alive_neighbour < 2){
                    cur[i_row + j_col] = DEAD;
                }
                /* 2. Any live cell with two or three live neighbours lives on to the next generation.  */
                if(old[i_row + j_col] == ALIVE && (num_alive_neighbour == 2 || num_alive_neighbour == 3)){
                    cur[i_row + j_col] = ALIVE;
                }
                /* 3. Any live cell with more than three live neighbours dies, as if by overpopulation. */
                if(num_alive_neighbour > 3){
                    cur[i_row + j_col] = DEAD;
                }
                /* 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction. */
                if(old[i_row + j_col] == DEAD && num_alive_neighbour == 3){
                    cur[i_row + j_col] = ALIVE;
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
