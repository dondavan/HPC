#!/bin/bash -e
#SBATCH --partition=thin_course
#SBATCH -t 3:00 --ntasks=16

module purge
module load 2023
module load MPICH/4.1.2-GCC-12.3.0

mpirun ./Parallel_MPI -n10 -m10 -x3 -y3 -k0 -finput/glider.txt