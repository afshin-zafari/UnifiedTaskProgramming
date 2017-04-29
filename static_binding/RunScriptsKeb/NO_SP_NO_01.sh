#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_NO_SP_NO_01_1K80
#SBATCH -n 28
#SBATCH -N 1
#SBATCH --gres=gpu:k80:1
#SBATCH --time=00:05:00
#SBATCH --output=out_NO_SP_NO_01-%j.txt
#SBATCH --error=err_NO_SP_NO_01-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh
export STARPU_SCHED=dmdar
export STARPU_CALIBRATE=0
export STARPU_HOSTNAME=afshin_spu


set -x
P=1;B2=1;p=1;q=1;nt=28

timemout=60

app_path=${run_base}/bin
app=${app_path}/cholesky_grain_tag

JID=${SLURM_JOBID}

for NN in 60
do
	for x  in  4
	do
		N=$[$NN * 200] 		
		B1=$NN
		out="P${P}_N${N}_B${B1}_$JID.txt"
		app_params="-size $N -nblocks $B1"
		$app ${app_params}
		echo "B= $B1" >> out_NO_SP_NO_01-${JID}.txt
	done
done
grep -i -A 2 "size" out_NO_SP_NO_01-${JID}.txt >> no_sp_no_timings.txt


