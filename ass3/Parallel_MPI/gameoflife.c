#include <time.h>
#include <math.h>
#include <mpi.h>
#include <unistd.h>
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
    char(*restrict cur) = malloc(row * col * sizeof(char));
    char(*restrict old) = malloc(row * col * sizeof(char));
    if(cur == NULL || old == NULL)die("Malloc Failed");    /* Check if malloc succeed */

    /* Initialize Board */
    for(size_t i = 0; i < row; i++){
        for(size_t j = 0; j < col; j++){
            cur[i*col + j]    = DEAD;
            old[i*col + j]    = DEAD;
        }
    }
    /* Read-in Initial Board*/
    read_board(p->input_fname, row, col, p->Y, p->X, old);
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

    /* Communicating with other nodes*/
    size_t prev = MPI_rank-1;
    size_t next = MPI_rank+1;
    MPI_Status stats[4];
    MPI_Request reqs[4];
    char * send_buf_1 = malloc(col * sizeof(char)); /* Buffer for MPI send*/
    char * send_buf_2 = malloc(col * sizeof(char)); /* Buffer for MPI send*/
    char * recv_buf_1 = malloc(col * sizeof(char)); /* Buffer for MPI recv*/
    char * recv_buf_2 = malloc(col * sizeof(char)); /* Buffer for MPI recv*/
    
    /* Distribute Data Through Row Distribution */
    size_t chuck_size;
    size_t row_start, row_end, col_start, col_end;
    chuck_size = row / MPI_world_size;

    /* Assign Cell Bounday for Each Node */
    row_start = MPI_rank * chuck_size + 1;      /* Tight Boundary */
    row_end   = row_start + chuck_size - 1;     /* Tight Boundary */
    col_start   = 1; col_end     = col-1;       /* Border with permant DEAD cell, so we don't iterate over them*/

    if(MPI_rank == 0)row_start=1;                 /* Border with permant DEAD cell, so we don't iterate over them*/
    if(MPI_rank == MPI_world_size-1)row_end=row-1;  /* Border with permant DEAD cell, so we don't iterate over them*/


    printf("My rank: %d, Row start: %d, Row end:%d \n",MPI_rank,row_start,row_end);

    /**************************************************/
    /*                  Run Iteration                 */
    /**************************************************/
    size_t iter, i_row, j_col,j;
    size_t num_alive_neighbour;
    double starttime, endtime;
    starttime = MPI_Wtime();
    
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

        
        if(MPI_rank!=0)                 MPI_Wait(&reqs[2],&stats[2]);
        if(MPI_rank!=MPI_world_size-1)  MPI_Wait(&reqs[3],&stats[3]);

        if(MPI_rank!=0)                 for(j = 0; j < col; j++)old[(row_start-1)*col + j] = recv_buf_1[j];
        if(MPI_rank!=MPI_world_size-1)  for(j = 0; j < col; j++)old[(row_end+1)*col   + j] = recv_buf_2[j];
        

        /* Iterate Over Cells */
        for(i_row = row_start; i_row <= row_end; i_row++){
            for(j_col = col_start; j_col <= col_end; j_col++){
                
                num_alive_neighbour = 0;
                /* Count Alive Neighbours Around Current Cell */
                num_alive_neighbour = old[(i_row-1)*col + j_col-1] + old[(i_row-1)*col + j_col] + old[(i_row-1)*col + j_col+1] + 
                                      old[(i_row  )*col + j_col-1] +                            + old[(i_row  )*col + j_col+1] +
                                      old[(i_row+1)*col + j_col-1] + old[(i_row+1)*col + j_col] + old[(i_row+1)*col + j_col+1] ;


                cur[i_row*col + j_col] = DEAD;
                /* Apply Rules */
                if(old[i_row*col + j_col] == ALIVE){
                    /* 1. Any live cell with fewer than two live neighbours dies, as if by underpopulation. */
                    if(num_alive_neighbour < 2){
                        cur[i_row*col + j_col] = DEAD;
                    }
                    /* 3. Any live cell with more than three live neighbours dies, as if by overpopulation. */
                    else if(num_alive_neighbour > 3){
                        cur[i_row*col + j_col] = DEAD;
                    }
                    /* 2. Any live cell with two or three live neighbours lives on to the next generation.  */
                    else{
                        cur[i_row*col + j_col] = ALIVE;
                    }
                }
                else{
                    /* 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction. */
                    if(num_alive_neighbour == 3){
                        cur[i_row*col + j_col] = ALIVE;
                    }
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

    endtime   = MPI_Wtime();
    
    
    
    /* Gather Cells From Other Node to Master Node */
    if(MPI_rank != 0){

        char * gather_partition = malloc(row * col * sizeof(char));
        size_t p = 0;   /* Sequential Pointer */
        for(i_row = 0; i_row < row; i_row++){
            for(j_col = 0; j_col < col; j_col++){
                gather_partition[p] = old[(i_row)*col + j_col];
                p++;
            }
        
        }
        MPI_Send(gather_partition, row * col, MPI_BYTE, 0, MPI_rank, MPI_COMM_WORLD);

    }
    else{
        char * gather_partition = malloc(row * col * sizeof(char));
        MPI_Status stats;
        size_t p;   /* Sequential Pointer */
        for(size_t target_rank = 1; target_rank < MPI_world_size; target_rank++){
            MPI_Recv(gather_partition,row * col, MPI_BYTE, target_rank, target_rank, MPI_COMM_WORLD,&stats);

            row_start = MPI_rank * chuck_size + 1;      /* Tight Boundary */
            row_end   = row_start + chuck_size - 1;     /* Tight Boundary */
            col_start   = 1; col_end     = col-1;       /* Border with permant DEAD cell, so we don't iterate over them*/
            if(MPI_rank == MPI_world_size-1)row_end=row-1;  /* Border with permant DEAD cell, so we don't iterate over them*/

            for(i_row = 0; i_row < row; i_row++){
                for(j_col = 0; j_col < col; j_col++){
                    printf("%d",gather_partition[p]);
                    p++;
                }
                printf("\n");
            }
            
        }

    }


    if(MPI_rank == 0) {
        printf("That took %f seconds\n",endtime-starttime);
        for (int i = 0; i < row ; i++){
            for(int j = 0; j < col; j++){
                printf("%d",old[i*col +j]);
            }
            printf("\n");
        }
    }
    //Output Board for Report
    if(MPI_rank==0){
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
    free(send_buf_1);
    free(send_buf_2);
    free(recv_buf_1);
    free(recv_buf_2);
    atexit(cleanup);
}
