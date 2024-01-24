#!/bin/bash
#SBATCH --job-name="pi"
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --time=00:20:00
#SBATCH --partition=thin_course
#SBATCH --output=pi_%j.out
#SBATCH --error=pi_%j.err

module purge
module load 2022
module load GCCcore-11.3.0

echo "OpenMP parallelism"
echo

for i in `seq 1 20`
do
    echo "======== Run : " $i
    export OMP_NUM_THREADS=32

    echo "CPUS: " $OMP_NUM_THREADS
    echo "CPUS: " $OMP_NUM_THREADS >&2

    ./pi -i31250000
    ./pi -i62500000
    ./pi -i125000000
    ./pi -i250000000
    ./pi -i500000000
    ./pi -i1000000000
    ./pi -i2000000000
    
    echo "DONE "
done
