#!/bin/bash
# Usage: runall.sh [<folder>] [<pattern to match>]  [<extra RapidFit options>]
make -C modules -j
currentdir=$(pwd)
if [ "$1" == "" ]
then
	fitfolders=(toystudies mcfits/phasespace mcfits/pwave datafits)
else
	fitfolders=($1)
fi
for folder in ${fitfolders[@]}
do
	cd $currentdir/$folder
	for file in $(ls *$2*xml)
	do
                mkdir -p FitResult_$(echo $file | sed 's/\.xml//g')
                cd FitResult_$(echo $file | sed 's/\.xml//g')
		fitting -f $file $3 | tee RapidFitOutput-$(date +"%Y%m%d_%H%M%S").log
		$currentdir/output/mergeprojections.sh
		$currentdir/output/compareresult.sh
	done
done
