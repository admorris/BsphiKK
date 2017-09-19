#!/bin/bash
# Usage: submitall_toys.sh <folder> <pattern to match> <number of jobs> <number of repeats per job>
source RFjobconfig.sh
make -C modules -j$(nproc) || exit 1
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
		for i in $(seq 0 $((${3}-1)))
		do
			submission_script=$(echo "submit_$i_$file" | sed 's/xml/sh/')
			cat <<-EOF > ${submission_script}
			#!/bin/bash
			#$ -N "j_${i}_$(echo $file | sed 's/.xml//')"
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
			export PATH=\$PATH:$currentdir/../RapidFit/bin:$currentdir/../bin
			# Move to the right folder
			mkdir -p FitResult_$(echo $file | sed 's/\.xml//g')
			cd FitResult_$(echo $file | sed 's/\.xml//g')
			for repeat in \`seq 0 $((${4}-1))\`
			do
				index=\$(uuidgen)
				outputdir=RapidFitOutput_\${index}
				mkdir \${outputdir}
				logfile=\${outputdir}/RapidFitOutput.log
				# Perform the fit
				fitting \${nThreadsFlag} ${nRepeatsFlag} -f ../${file} --useUUID --SendOutput \${outputdir} 2>&1| tee \${logfile}
			done
			EOF
			# Submit the jobs
			qsub ${submission_script}
			rm ${submission_script}
		done
	done
done

