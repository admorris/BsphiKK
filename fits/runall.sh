#!/bin/bash
# Usage: runall.sh [<folder>] [<pattern to match>]  [<extra RapidFit options>]
make -C modules -j
currentdir=$(pwd)
if [ "$1" == "" ]
then
	fitfolders=($(find src -mindepth 1 -type d | sed 's/src\///g'))
else
	fitfolders=($1)
fi
for folder in ${fitfolders[@]}
do
	cd $currentdir/results/$folder
	for file in $(ls *$2*xml)
	do
		# Move to the right folder
		mkdir -p FitResult_$(echo $file | sed 's/\.xml//g')
		cd FitResult_$(echo $file | sed 's/\.xml//g')
		# Perform the fit
		fitting --OverrideXML /RapidFit/FitFunction/Threads $(nproc) -f ../${file} --generateToyXML --MultiDimChi2 $3 | tee RapidFitOutput-$(date +"%Y%m%d_%H%M%S").log
		# Deal with the output
		$currentdir/output/mergeprojections.sh
		$currentdir/output/compareresult.sh
		#$currentdir/output/comparemoments.sh
		cd ..
	done
done
