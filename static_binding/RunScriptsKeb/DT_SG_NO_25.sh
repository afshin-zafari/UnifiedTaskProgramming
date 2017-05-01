#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_DT_SG_NO_25
#SBATCH -n 700
#SBATCH --exclusive
#SBATCH -N 25
#SBATCH --time=00:10:00
#SBATCH --output=out_DT_SG_NO_25-%j.txt
#SBATCH --error=err_DT_SG_NO_25-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh

set -x
P=25;p=5;q=5;nt=28;ipn=1;

timeout=2000

app_path=${run_base}/bin
app=${app_path}/utp_dt_sg_blas_release
JID=${SLURM_JOBID}

B2=10
for z in 400
do
	for B1 in 9
	do		
		N=$[$z * $B1 *$B2] 
		out="P${P}_N${N}_B${B1}_$JID.txt"
		mpi_params="-ordered-output -n $P -ppn $ipn -outfile-pattern  $out -errfile-pattern $out -l"
		app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout"
		mpirun ${mpi_params} $app ${app_params}
	done
done
grep -i "\[UTP\]" P*.txt > dt_sg_timings.txt
rm *file*.txt

