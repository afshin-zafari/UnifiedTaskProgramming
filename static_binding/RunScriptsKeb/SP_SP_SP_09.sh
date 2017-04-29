#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_SP_SP_SP_09
#SBATCH -n 252
#SBATCH --exclusive
#SBATCH -N 9
#SBATCH --gres=gpu:k80:1
#SBATCH --time=00:05:00
#SBATCH --output=out_SP_SP_SP_09-%j.txt
#SBATCH --error=err_SP_SP_SP_09-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh

set -x
nt=28
P=9;p=3;q=3;ipn=1

timemout=60

app_path=${run_base}/bin
app=${app_path}/mpi_cholesky_distributed

JID=${SLURM_JOBID}

export STARPU_SCHED=dmdar
export STARPU_CALIBRATE=1
export STARPU_HOSTNAME=afshin_spu



N=23000
B1=$[$N / 200]
app_params="-size $N -nblocks $B1 " #-dblockx $q -dblocky $p"
out="P${P}_N${N}_B${B1}_b${B2}_$JID.txt"
mpi_params="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out -errfile-pattern $out -l"
mpirun ${mpi_params} $app ${app_params}
