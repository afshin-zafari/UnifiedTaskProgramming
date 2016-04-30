#!/bin/bash
#SBATCH -A p2009014
#SBATCH -o UI-SG-GEMM-%j.out
#SBATCH -p devel
#SBATCH -t 00:10:00
#SBATCH -N 1
#SBATCH -n 16
#SBATCH -J UI-SG-GEMM

set -x
UI_GEMM=/pica/h1/afshin/Damavand/D3/apps/bin/Debug/apps
SG_GEMM=/pica/h1/afshin/Damavand/D3/sg_gemm/bin/Debug/sg_gemm

for M in 1024 #2048 4096
do
	for Z in 128 #16 32 64 128
	do
		B=$[$M/$Z]

		for i in $(seq 1 5)
		do
			#$UI_GEMM -M $M $B 1 -N 1 1 1 -P 1 -p 1 -q 1 -nt 16
			$SG_GEMM $Z $B 16
		done
	done
done

