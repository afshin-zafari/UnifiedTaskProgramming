#!/bin/bash

#SBATCH -A p2009014
#SBATCH -o UI-DT-GEMM-%j.out
#SBATCH -p devel
#SBATCH -t 00:02:00
#SBATCH -N 1
#SBATCH -n 16
#SBATCH -J UI-DT-GEMM

module load gcc/4.9 openmpi/1.8.1 cuda/7.5

set -x
B1=4
B2=2
N=2048
M=$N
timeout=100
P=2
p=2
q=1
ipn=2
nt=2
app_bo="./bin/utp_blas_only$1"
app_db="./bin/non_utp_dt_blas"
app_sb="./bin/utp_sg_blas$1"
app_dsb="./bin/utp_dt_sg_blas$1"
out="out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
out2="dsb_out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
mpi_params="-np $P --bind-to numa  --map-by numa --map-by ppr:$ipn:node --output-filename $out"



#-----DT BLAS 

mpirun ${mpi_params} ${app_db} ${app_params}
grep -i "error" $out*
