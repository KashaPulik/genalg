#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --job-name=single_core_task
#SBATCH --partition=release

cd $SLURM_SUBMIT_DIR

export OMP_NUM_THREADS=1

export SLURM_CPU_BIND=cores
export SLURM_CPU_BIND_LIST="0"

./app