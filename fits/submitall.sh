#!/bin/bash
# Usage: submitall.sh [<folder>] [<pattern to match>] [<extra RapidFit options>]
source RFjobconfig.sh
make -C modules -j
currentdir=$(pwd)
if [ "$1" == "" ]
then
	fitfolders=(toystudies mcfits/phasespace mcfits/pwave datafits/sweighted datafits/withbackground)
else
	fitfolders=($1)
fi
for folder in ${fitfolders[@]}
do
	cd $currentdir/$folder
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
		# Re-make the XML file to pick up the number of threads this machine has
		rm $file && make -C $currentdir/modules ../$folder/$file
		# Set up the environment
		source $currentdir/RFjobconfig.sh
		EOF
		echo -e "$SetupEnvironment" >> ${submission_script}
		cat <<-EOF >> ${submission_script}
		export PATH=\$PATH:\$RapidFitDir/bin
		# Move to the right folder
		mkdir -p FitResult_$(echo $file | sed 's/\.xml//g')
		cd FitResult_$(echo $file | sed 's/\.xml//g')
		# Perform the fit
		fitting -f ../${file} --generateToyXML --calculateFitFractions $3 | tee RapidFitOutput-\$(date +"%Y%m%d_%H%M%S").log
		# Deal with the output
		$currentdir/output/mergeprojections.sh
		$currentdir/output/compareresult.sh
		EOF
		# Submit the jobs
		qsub ${submission_script}
		rm ${submission_script}
	done
done

