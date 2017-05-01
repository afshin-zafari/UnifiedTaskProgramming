#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_SP_SP_NO_25
#SBATCH -n 448
#SBATCH --exclusive
#SBATCH -N 16
#SBATCH --time=00:15:00
#SBATCH --output=out_SP_SP_NO_25-%j.txt
#SBATCH --error=err_SP_SP_NO_25-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh

set -x
nt=28
P=25;p=5;q=5;ipn=1

timemout=60

app_path=${run_base}/bin
app=${app_path}/mpi_cholesky_distributed

JID=${SLURM_JOBID}

export STARPU_SCHED=dmdar
export STARPU_CALIBRATE=1
export STARPU_HOSTNAME=afshin_spu



N=36000
B1=$[$N / 400]
app_params="-size $N -nblocks $B1 " #-dblockx $q -dblocky $p"
out="P${P}_N${N}_B${B1}_b${B2}.txt"
mpi_params="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out -errfile-pattern $out -l"
mpirun ${mpi_params} $app ${app_params}
