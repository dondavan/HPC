#!/bin/bash

module load 2022
module load GCC/11.3.0
gcc -fopenmp -o pi pi.c -lm