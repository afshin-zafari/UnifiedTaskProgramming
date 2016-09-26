#!/bin/bash

#SBATCH -A p2009014
#SBATCH -o UI-DT-DOT-%j.out
#SBATCH -p devel
#SBATCH -t 00:02:00
#SBATCH -N 1
#SBATCH -n 16
#SBATCH -J UI-DT-GEMM

module load gcc/4.9 openmpi/1.8.1 cuda/7.5
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../dispatcher/bin/Debug
mkdir -p log
set -x
SG_PATH=/pica/h1/afshin/Damavand/D3/sg_scheduler/bin/Debug/sg_scheduler.so
BLAS_PATH=""
CUBLAS_PATH=/pica/h1/afshin/Damavand/D3/cuBLAS_scheduler/CUBLAS.so

opt_string(){
    DT="--scheduler${DT_INDEX}=DuctTeip"
    SG="--scheduler${SG_INDEX}=SuperGlue --sched-lib${SG_INDEX}=${SG_PATH}"
    BLAS="--scheduler${BLAS_INDEX}=cpuBLAS --sched-lib${BLAS_INDEX}=${BLAS_PATH}"
    cuBLAS="--scheduler${CUBLAS_INDEX}=cuBLAS --sched-lib${CUBLAS_INDEX}=${CUBLAS_PATH}"
    MAGMA=""
}
cfg_SG_BLAS(){
    SG_INDEX=1
    BLAS_INDEX=2
    opt_string
    SCH=" $SG $BLAS"
}
cfg_DT_BLAS(){
    DT_INDEX=1    
    BLAS_INDEX=2
    opt_string
    SCH=" $DT $BLAS "
}
cfg_DT_SG_BLAS(){
    DT_INDEX=1    
    SG_INDEX=2
    BLAS_INDEX=3
    opt_string
    SCH=" $DT $SG $BLAS "
}
cfg_DT_cuBLAS(){
    DT_INDEX=1    
    CUBLAS_INDEX=2
    opt_string
    SCH=" $DT $cuBLAS "
}
cfg_sched_graph(){
    s= "DT ( ( SG cuBLAS)  cpuBLAS )"
}
#rm gemm* core.*

B1=4
B2=1
M=24
N=24
timeout=3
P=2
p=2
q=1
ipn=2
nt=16
app=../bin/ugemm.sttest
cfg_DT_SG_BLAS
mpirun -np $P --bind-to numa  --map-by numa --map-by ppr:$ipn:node --output-filename "./test/gemm_sg_blas_ipn_${ipn}_${M}_${B1}_${SLURM_JOB_ID}.out" $app -M $M $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout $SCH 
rm *.txt

cfg_DT_BLAS
#mpirun -np 2 --output-filename gemm_dt_blas $app -M $M $B1 $B2 -N $M $B1 $B2 -P 2 -p 2 -q 1 -t 2 -T $timeout $SCH 
rm *.txt

cfg_SG_BLAS
#mpirun -np 1 --output-filename gemm_sg_blas $app -M $M $B1 $B2 -N $M $B1 $B2 -P 2 -p 2 -q 1 -t 2 -T $timeout $SCH 
rm *.txt

cfg_DT_SG_BLAS
#mpirun -np 2 --output-filename gemm_dt_sg_blas $app -M $M $B1 2 -N $M $B1 2 -P 2 -p 2 -q 1 -t 2 -T $timeout $SCH 
rm *.txt


cfg_DT_cuBLAS
#mpirun -np 1 --map-by ppr:1:node --output-filename gemm_one_dt_cublas $app -M $M $B1 1 -N $M $B1 1 -P 1 -p 1 -q 1 -t 2 -T $timeout $SCH 
rm *.txt


