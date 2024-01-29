#!/bin/bash -e
#SBATCH --partition=thin_course
#SBATCH -t 10:00 --ntasks=120

module purge
module load 2023
module load MPICH/4.1.2-GCC-12.3.0

for i in `seq 1 10`
do
    echo "======== Run : " $i
    mpirun ./Parallel_MPI -n3000 -m3000 -x1500 -y1500 -i5000 -finput/grower.txt
done
