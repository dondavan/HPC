#include "stdint.h"
#include <stdlib.h>
#include <stdio.h>


int main(){

    FILE *f = fopen("target.txt", "r");
    if (f == NULL)exit(1);

    unsigned row, col, v;

    /* Reading Amount of Alive Cells*/
    fscanf(f, "%u %u", &row, &col);
    printf("row: %u, col: %u\n",row,col);
    
    int count = 0;
    char c;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if (fscanf(f, "%c", &c) != 1) exit(1);
            v = (int)c - 48;
            if(v == 1)count++;
        }
    }

    printf("Count: %d", count);

    fclose(f);

}