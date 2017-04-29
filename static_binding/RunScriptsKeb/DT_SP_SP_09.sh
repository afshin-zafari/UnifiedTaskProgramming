#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_DT_SP_SP_09
#SBATCH -n 252
#SBATCH --exclusive
#SBATCH -N 9
#SBATCH --gres=gpu:k80:1
#SBATCH --time=00:05:00
#SBATCH --output=out_DT_SP_SP_09-%j.txt
#SBATCH --error=err_DT_SP_SSP_09-%j.txt


run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh

set -x
P=9;p=3;q=3;nt=28;ipn=1;

timeout=1000

app_path=${run_base}/bin
app=${app_path}/utp_dt_spu_blas_release
JID=${SLURM_JOBID}


export STARPU_SCHED=dmdar
export STARPU_CALIBRATE=1
export STARPU_HOSTNAME=afshin_spu
B2=5
for z in 200
do
	for B1 in 23
	do		
		N=$[$z * $B1 *$B2] 
		out="P${P}_N${N}_B${B1}.txt"
		mpi_params="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out -errfile-pattern $out -l"
		app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout"
		mpirun ${mpi_params} $app ${app_params}
	done
done
grep -i "\[UTP\]" P*.txt > dt_sp_sp_timings.txt
rm *file*.txt