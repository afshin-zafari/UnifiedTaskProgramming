#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_UTP_NO_SP_SP_01_1K80
#SBATCH -n 28
#SBATCH -N 1
#SBATCH --gres=gpu:k80:1
#SBATCH --time=00:05:00
#SBATCH --output=out_UTP_NO_SP_SP_01-%j.txt
#SBATCH --error=err_UTP_NO_SP_SP_01-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh
export STARPU_SCHED=dmdar
export STARPU_CALIBRATE=0
export STARPU_HOSTNAME=afshin_spu


set -x
P=1;B2=1;p=1;q=1;nt=28

timemout=60

app_path=${run_base}/bin
app=${app_path}/utp_spu_blas #_release

JID=${SLURM_JOBID}

for NN in 45
do
	N=$[$NN * 800] 
	B1=$NN
	out="UTP_P${P}_N${N}_B${B1}_$JID.txt"
	app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
	$app ${app_params}
done
grep -i -A 2 "\[UTP\]" UTP_P* >> utp_no_sp_sp_timings.txt


