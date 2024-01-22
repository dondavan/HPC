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
    read_board(p->input_fname, row, col, 3, 3, old);
    /* Output Board for Report*/
    {
        r->niter    = 99;
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
    size_t iter, i_row, j_col,j;
    size_t num_alive_neighbour;

    /* Communicating with other nodes*/
    size_t prev = MPI_rank-1;
    size_t next = MPI_rank+1;
    MPI_Status stats[4];
    MPI_Request reqs[4];
    char * send_buf_1 = malloc(col * sizeof(char)); /* Buffer for MPI send*/
    char * send_buf_2 = malloc(col * sizeof(char)); /* Buffer for MPI send*/
    char * recv_buf_1 = malloc(col * sizeof(char)); /* Buffer for MPI recv*/
    char * recv_buf_2 = malloc(col * sizeof(char)); /* Buffer for MPI recv*/
    
    for(iter = 0; iter < p->maxiter; iter ++){

        /* Send Lower row_start-1 And row_end+1 */
        for(j = 0; j < col; j++){
            send_buf_1[j] = old[row_start*col + j];
            send_buf_2[j] = old[row_end*col   + j];
        }
        if(MPI_rank!=0)                 MPI_Isend(send_buf_1,col,MPI_BYTE,prev,1,MPI_COMM_WORLD, &reqs[0]);
        if(MPI_rank!=MPI_world_size-1)  MPI_Isend(send_buf_2,col,MPI_BYTE,next,2,MPI_COMM_WORLD, &reqs[1]);
                
        /* Sync 'old' Cells */
        if(MPI_rank!=0)                 MPI_Irecv(recv_buf_1,col,MPI_BYTE,prev,2,MPI_COMM_WORLD, &reqs[2]);
        if(MPI_rank!=MPI_world_size-1)  MPI_Irecv(recv_buf_2,col,MPI_BYTE,next,1,MPI_COMM_WORLD, &reqs[3]);
        for(j = 0; j < col; j++){
            old[(row_start-1)*col + j] = recv_buf_1[j];
            old[(row_end+1)*col   + j] = recv_buf_2[j];
        }

        if(MPI_rank!=0)                 MPI_Wait(&reqs[2],&stats[2]);
        if(MPI_rank!=MPI_world_size-1)  MPI_Wait(&reqs[3],&stats[3]);
        if(MPI_rank==1)for(int i =0 ;i <col;i++) printf("%d",recv_buf_1[i]);

        
        
        MPI_Barrier(MPI_COMM_WORLD);
        if(MPI_rank==1)printf("\n");
    }

    printf("From %d \n",MPI_rank); 
    /* Output Board for Report*/
    for(i_row = row_start; i_row <= row_end; i_row++){
            for(j_col = col_start; j_col <= col_end; j_col++){
              printf("%hhu",old[i_row*col + j_col]);  
            }
        }
    /* Output Board for Report
    if(MPI_rank==0){
        r->niter    = iter;
        r->row      = p->N;
        r->col      = p->M;
        r->board    = old;
        report_results(r);
    }*/


    /**************************************************/
    /*                   Cleaning Up                  */
    /**************************************************/
    free(old);
    free(cur);
    free(send_buf_1);
    free(send_buf_2);
    free(recv_buf_1);
    free(recv_buf_2);
    atexit(cleanup);
}
