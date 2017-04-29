#!/bin/bash

write_batch(){
    echo "#!/bin/bash" > ${batch_file}
    echo ""                         >> ${batch_file}
    echo "#SBATCH -A SNIC2016-3-74" >> ${batch_file}
    echo "#SBATCH -J $sbjob"        >> ${batch_file}
    echo "#SBATCH -n $[$P * 28]"    >> ${batch_file}
    echo "#SBATCH -N $P"            >> ${batch_file}
    if [ "$GPU" = "SP" ];then
	echo "#SBATCH --gres=gpu:k80:1" >> ${batch_file}
    fi
    echo "#SBATCH --time=00:05:00"  >> ${batch_file}
    echo "#SBATCH --output=$sbout"  >> ${batch_file}
    echo "#SBATCH --error=$sberr"   >> ${batch_file}
}


for MPI in NO DT SP
do
    for CPU in SP SG
    do
	for GPU in NO SP
	do
	    exper=${MPI}_${CPU}_${GPU}
	    for P in 1 2 4 8 16
	    do
		batch_file="${exper}_0$P.sh"
		pars="${exper}_0$P"
		sbout="out_${pars}.txt"
		sberr="err_${pars}.txt"
		sbjob="job_${pars}"
		write_batch
	    done
	done
	
    done
done
rm DT_SG_SP*.sh
rm NO_SG_SP*.sh
rm SP_SG_SP*.sh
rm NO_SP_SP*.sh
rm SP_SG_NO*.sh
rm NO_*2.sh
rm NO_*4.sh
rm NO_*8.sh
rm NO_*16.sh