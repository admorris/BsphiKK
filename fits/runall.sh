#!/bin/bash
# Usage: runall.sh [<folder>] [<pattern to match>]  [<extra RapidFit options>]
make -C modules -j || exit 1
currentdir=$(pwd)
if [ "$1" == "" ]
then
	fitfolders=($(find src -mindepth 1 -type d | sed 's/src/results/g'))
else
	fitfolders=($1)
fi
for folder in ${fitfolders[@]}
do
	cd $currentdir/$folder
	for file in $(ls *$2*xml)
	do
		# Move to the right folder
		mkdir -p FitResult_$(echo $file | sed 's/\.xml//g')
		cd FitResult_$(echo $file | sed 's/\.xml//g')
		# Perform the fit
		logfile=RapidFitOutput-$(date +"%Y%m%d_%H%M%S").log
		if [[ ${NSLOTS} ]]
		then
			nThreadsFlag="--OverrideXML /RapidFit/FitFunction/Threads ${NSLOTS}"
		fi
		fitting ${nThreadsFlag} -f ../${file} --generateToyXML --MultiDimChi2 --ForceContinue $3 2>&1| tee ${logfile}
		# Deal with the output
		$currentdir/scripts/mergeprojections.sh 2>&1| tee -a ${logfile}
		$currentdir/scripts/compareresult.sh 2>&1| tee -a ${logfile}
		$currentdir/scripts/comparemoments.sh 2>&1| tee -a ${logfile}
		$currentdir/scripts/calculatefitfractions.sh 2>&1| tee -a ${logfile}
		cd ..
	done
done
