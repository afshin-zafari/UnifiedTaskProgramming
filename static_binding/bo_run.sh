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
B2=4
N=32
M=$N
timeout=10
P=1
p=1
q=1
ipn=1
nt=2
app_bo="./bin/utp_blas_only"
app_db="./bin/utp_dt_blas"
app_sb="./bin/utp_sg_blas"
app_dsb="./bin/utp_dt_sg_blas"
out="out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
out2="dsb_out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
mpi_params1="-np $P --bind-to numa  --map-by numa --map-by ppr:$ipn:node --output-filename $out"
mpi_params2="-np $P --bind-to numa  --map-by numa --map-by ppr:$ipn:node --output-filename $out2"


#-----BLAS ONLY
echo "========================================================================="
echo "========================================================================="
$app_bo ${app_params} > bo_${out}

#-----SG BLAS 
echo "========================================================================="
echo "========================================================================="
$app_sb ${app_params} > sb_${out}

#-----DT BLAS 
echo "========================================================================="
echo "========================================================================="
mpirun ${mpi_params1} ${app_db} ${app_params}

#-----DT SG BLAS 
echo "========================================================================="
echo "========================================================================="
mpirun ${mpi_params2} ${app_dsb} ${app_params}
grep -i "timeout" dsb*.txt*



