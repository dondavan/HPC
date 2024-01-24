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
