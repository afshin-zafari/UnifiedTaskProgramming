#!/bin/bash

#SBATCH -A SNIC2016-3-74
#SBATCH -J job_NO_SG_NO_01
#SBATCH -n 28
#SBATCH -N 1
#SBATCH --time=00:05:00
#SBATCH --output=out_NO_SG_NO_01-%j.txt
#SBATCH --error=err_NO_SG_NO_01-%j.txt

run_base=/home/a/afshin/pfs/UTP_Cholesky_Runs
source ${run_base}/main.sh

set -x
P=1;B2=1;p=1;q=1;nt=28

timemout=60

app_path=${run_base}/bin
app=${app_path}/utp_sg_blas   #_release
JID=${SLURM_JOBID}
for z in 180 200
do
	for NN in  4 5 6 8 
	do
		N=$[$NN * 3600] 
		B1=$[$N / $z]
		out="P${P}_N${N}_B${B1}.txt"
		app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
		$app ${app_params}
	done
done
grep -i "\[UTP\]" out_NO_SG_NO_01-${JID}.txt >> timings.txt

exit

for N in 1024 2048 4096 8192 16384 
do
    for B1 in 8 16 32 64 128
    do
	out="P${P}_N${N}_B${B1}.txt"
	app_params="-M $N $B1 $B2 -N $N $B1 $B2 -P $P -p $p -q $q -t $nt -T $timeout >$out"
	$app ${app_params}
    done
done
