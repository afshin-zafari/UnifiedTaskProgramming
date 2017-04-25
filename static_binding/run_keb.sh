#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J SPU_N01_T28
#SBATCH -n 28
#SBATCH -N 1
#SBATCH --gres=gpu:k80:1
#SBATCH --time=00:05:00
#SBATCH --output=SPU_N01_T28_%j.OUT 
#SBATCH --error=SPU_N01_T28_%j.ERR


#zSBATCH -A SNIC2016-3-74
#zSBATCH -J UI-DT-GEMM
#zSBATCH -N 1
#zSBATCH -n 28
#zSBATCH --gres=gpu:k80:1
#zSBATCH --time=00:05:00
#zSBATCH --output=UI-DT-GEMM-%j.out
#zSBATCH --error=UI-DT-GEMM-%j.err


ml add intelcuda
LD_LIBRARY_PATH=/home/a/afshin/pfs/StarPU/starpu-1.2.0/BUILD/lib:${LD_LIBRARY_PATH}
LD_LIBRARY_PATH=/hpc2n/eb/software/Compiler/GCC/5.4.0-2.26/OpenBLAS/0.2.18-LAPACK-3.6.1/lib:${LD_LIBRARY_PATH}


set -x

B1=4
B2=3
N=480
M=$N
timeout=10
P=2
p=2
q=1
ipn=2
nt=2
app_bo="./bin/utp_blas_only$1"
app_db="./bin/utp_dt_blas$1"
app_sb="./bin/utp_sg_blas$1"
app_dsb="./bin/utp_dt_sg_blas"
app_spub="./bin/utp_spu_blas"
app_dspub="./bin/utp_dt_spu_blas"
out="out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
out2="dsb_out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
out3="dspub_out_${M}_${B1}_${B2}_${p}_${q}_${nt}.txt"
app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
app_params3="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >${out}_dspub"
mpi_params1="-np $P --bind-to numa  --map-by numa --map-by ppr:$ipn:node --output-filename $out"
mpi_params2="-np $P --bind-to numa  --map-by numa --map-by ppr:$ipn:node --output-filename $out2"
mpi_params1="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out -errfile-pattern $out  -l"
mpi_params2="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out2 -errfile-pattern $out2  -l"
mpi_params3="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out3 -errfile-pattern $out3 -l"


#-----SPU BLAS 
echo "========================================================================="
echo "========================================================================="
$app_spub ${app_params} > spub_${out}

#-----DT SPU BLAS 
echo "========================================================================="
echo "========================================================================="
#mpirun ${mpi_params3} ${app_dspub} ${app_params3}
grep -i "error" dspub*.txt*
grep -i "\[0\]" ${out3} > p0_out.txt
grep -i "\[1\]" ${out3} > p1_out.txt

#-----DT SG BLAS 
echo "========================================================================="
echo "========================================================================="
#mpirun ${mpi_params2} ${app_dsb} ${app_params}

exit


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
grep -i "error" dsb*.txt*



