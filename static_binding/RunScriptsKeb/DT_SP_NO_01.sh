#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_DT_SP_NO_01
#SBATCH -n 28
#SBATCH --exclusive
#SBATCH -N 1
#SBATCH --time=00:05:00
#SBATCH --output=out_DT_SP_NO_01-%j.txt
#SBATCH --error=err_DT_SP_NO_01-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh

set -x
P=1;p=1;q=1;nt=28;ipn=1;

timeout=50

app_path=${run_base}/bin
app=${app_path}/utp_dt_spu_blas_release
JID=${SLURM_JOBID}

export STARPU_SCHED=dmdar
export STARPU_CALIBRATE=0
export STARPU_HOSTNAME=afshin_spu


B2=20
for z in 400
do
	for B1 in 5
	do		
		N=$[$z * $B1 *$B2] 
		out="P${P}_N${N}_B${B1}_$JID.txt"
		mpi_params="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out -errfile-pattern $out -l"
		app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout"
		mpirun ${mpi_params} $app ${app_params}
	done
done
grep -i "\[UTP\]" P*.txt > dt_sp_timings.txt
rm *file*.txt
