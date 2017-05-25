#!/bin/bash
# Usage: submitall.sh [<folder>] [<pattern to match>] [<extra RapidFit options>]
source RFjobconfig.sh
make -C modules -j$(nproc)
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
		submission_script=$(echo "submit_$file" | sed 's/xml/sh/')
		cat <<-EOF > ${submission_script}
		#!/bin/bash
		#$ -N "j_$(echo $file | sed 's/.xml//')"
		#$ -l h_rt=24:00:00
		#$ -l h_vmem=4G
		$ParallelEnv
		#$ -cwd
		#$ -hold_jid buildRapidFit
		if [[ \${NSLOTS} ]]
		then
			nThreadsFlag="--OverrideXML /RapidFit/FitFunction/Threads \${NSLOTS}"
		fi
		# Set up the environment
		source $currentdir/RFjobconfig.sh
		EOF
		echo -e "$SetupEnvironment" >> ${submission_script}
		cat <<-EOF >> ${submission_script}
		export PATH=\$PATH:\$RapidFitDir/bin:$currentdir/../bin
		# Move to the right folder
		mkdir -p FitResult_$(echo $file | sed 's/\.xml//g')
		cd FitResult_$(echo $file | sed 's/\.xml//g')
		# Perform the fit
		logfile=RapidFitOutput-\$(date +"%Y%m%d_%H%M%S").log
		fitting ${nThreadsFlag} -f ../${file} --generateToyXML --MultiDimChi2 $3 2>&1| tee \${logfile}
		# Deal with the output
		$currentdir/scripts/mergeprojections.sh 2>&1| tee -a \${logfile}
		$currentdir/scripts/compareresult.sh 2>&1| tee -a \${logfile}
		$currentdir/scripts/comparemoments.sh 2>&1| tee -a \${logfile}
		$currentdir/scripts/calculatefitfractions.sh 2>&1| tee -a \${logfile}
		EOF
		# Submit the jobs
		qsub ${submission_script}
		rm ${submission_script}
	done
done

