#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_UTP_NO_SP_NO_01_1K80
#SBATCH -n 28
#SBATCH -N 1
#SBATCH --time=00:05:00
#SBATCH --output=out_UTP_NO_SP_NO_01-%j.txt
#SBATCH --error=err_UTP_NO_SP_NO_01-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh
export STARPU_SCHED=dmdar
export STARPU_CALIBRATE=0
export STARPU_HOSTNAME=afshin_spu


set -x
P=1;B2=1;p=1;q=1;nt=28

timemout=60

app_path=${run_base}/bin
app=${app_path}/utp_spu_blas_release

JID=${SLURM_JOBID}

for NN in 10 15 20 25 30 40 60 80 100 120 140 150
do
	N=$[$NN * 200] 		
	B1=$NN
	out="UTP_P${P}_N${N}_B${B1}_$JID.txt"
	app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
	$app ${app_params}
	echo "B= $B1" >> out_NO_SP_NO_01-${JID}.txt
done
grep -i -A 2 "size" UTP_NO_SP_NO_01-${JID}.txt >> utp_no_sp_no_timings.txt


