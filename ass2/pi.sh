#!/bin/bash
#SBATCH --job-name="pi"
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=32
#SBATCH --time=00:20:00
#SBATCH --partition=thin_course
#SBATCH --output=pi_%j.out
#SBATCH --error=pi_%j.err



for i in `seq 1 20`
do
    echo "======== Run : " $i
    for ncores in `seq 8 4 48`

    do
      export OMP_NUM_THREADS=$ncores

      echo "CPUS: " $OMP_NUM_THREADS
      echo "CPUS: " $OMP_NUM_THREADS >&2
      ./pi -i1000000000
      echo "DONE "
    done
done

