#!/bin/bash

#SBATCH -A p2009014
#SBATCH -o UI-DT-GEMM-%j.out
#SBATCH -p devel
#SBATCH -t 00:02:00
#SBATCH -N 1
#SBATCH -n 16
#SBATCH -J UI-DT-GEMM

module load gcc/4.9 openmpi/1.8.1 

set -x
B1=8
B2=2
N=1024
M=$N
timeout=100
P=2
p=2
q=1
ipn=2
nt=2

app_dsb="./bin/non_utp_dt_sg_blas_release"
out="out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
mpi_params="-np $P --bind-to numa  --map-by numa --map-by ppr:$ipn:node --output-filename $out"



#-----DT SG BLAS 

mpirun ${mpi_params} ${app_dsb} ${app_params}
grep -i "error" $out*
