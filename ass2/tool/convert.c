#include "stdint.h"
#include <stdlib.h>
#include <stdio.h>

#include "grower.h"
#include "beehive.h"
#include "glider.h"

int main(){

    int height = GLIDER_HEIGHT;
    int width  = GLIDER_WIDTH;

    FILE *f = fopen("glider.txt", "w");
    if (f == NULL)exit(1);
    
    int count = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(glider[i][j] == 1)count++;
        }
    }

    fprintf(f, "%d\n", count); 

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(glider[i][j] == 1)fprintf(f, "%d %d\n", i, j); 
        }
    }
    fclose(f);

}
