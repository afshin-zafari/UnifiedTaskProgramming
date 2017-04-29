#!/bin/bash

src=/home/a/afshin/pfs/UTP_Cholesky_Runs
dst=/home/a/afshin/src/UnifiedTaskProgramming/static_binding/RunScriptsKeb

cd $src
set -x

src_files=$(find . -name \*.sh)
for f in ${src_files}
do
	cp $f $dst
done

cd $dst
cd ..