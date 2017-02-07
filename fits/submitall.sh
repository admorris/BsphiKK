#!/bin/bash
# Usage: submitall.sh [<folder>] [<pattern to match>] [<extra RapidFit options>]
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
		submission_script=$(echo "submit_$file" | sed 's/xml/sh/')
		runtimeoption=$(grep -h --color=never "\-l h_rt=" $file)
		if [ "$runtimeoption" == "" ]
		then
			runtimeoption=" #$ -l h_rt=00:20:00"
		fi
		cat <<-EOF > ${submission_script}
		#!/bin/bash
		#$ -N "j_$(echo $file | sed 's/.xml//')"
		$runtimeoption
		#$ -l h_vmem=1G
		#$ -pe mpi 16
		#$ -cwd
		# Set up the environment
		export PATH=\$PATH:~/RapidFit/bin
		. /etc/profile.d/modules.sh
		module load root
		module load igmm/apps/texlive
		# Move to the right folder
		mkdir -p FitResult_$(echo $file | sed 's/\.xml//g')
		cd FitResult_$(echo $file | sed 's/\.xml//g')
		# Perform the fit
		fitting -f ../${file} $3 | tee RapidFitOutput-\$(date +"%Y%m%d_%H%M%S").log
		# Deal with the output
		$currentdir/output/mergeprojections.sh
		$currentdir/output/compareresult.sh
		EOF
		# Submit the jobs
		qsub ${submission_script}
		rm ${submission_script}
	done
done

