#!/bin/bash
# Usage: toyjobs.sh <folder> [<timestamp>]
source RFjobconfig.sh
currentdir=$(pwd)
if [ "$1" == "" ]
then
	echo "Please provide a folder containing the RapidFit outputXMLFile"
	exit 0
else
	folder=${1}
fi
if [ "$2" == "" ]
then
	recentxml=$(find $folder -type f -name "outputXMLFile*" | sort | tail -1)
	timestamp=$(echo ${recentxml} | sed -r 's/.*outputXMLFile(.*)\.xml/\1/')
else
	recentxml="outputXMLFile${2}.xml"
	timestamp=${2}
fi
for i in `seq 0 15`
do
	cd ${currentdir}/${folder}
	workingfolder="acceptancejob_${timestamp}_${i}"
	mkdir -p ${workingfolder}
	cd ${workingfolder}
	submission_script="submit.sh"
	cat <<-EOF > ${submission_script}
	#!/bin/bash
	#$ -N "j_toy_acc_${timestamp}_${i}"
	#$ -l h_rt=24:00:00
	#$ -l h_vmem=4G
	#$ -l h='!morar2.ph.ed.ac.uk'
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
	export PATH=\$PATH:\$RapidFitDir/bin
	for acc in \`seq 0 7\`
	do
		index=\$(($i*8+\$acc))
		newXMLfile=\$(echo "${recentxml}" | sed "s/\./_\${index}/")
		sed "s/AcceptanceEntry:0/AcceptanceEntry:\${index}/" ${recentxml} > \$newXMLfile
		# Perform the fit
		logfile=RapidFitOutput-\$(date +"%Y%m%d_%H%M%S")_acceptance_\${index}.log
		fitting ${nThreadsFlag} -f ../${file} --OverrideXML /RapidFit/ToFit/DataSet/NumberEvents 100000 --generateToyXML --MultiDimChi2 --ForceContinue $3 2>&1| tee \${logfile}
		$currentdir/scripts/calculatefitfractions.sh 2>&1| tee -a \${logfile}
	done
	EOF
	# Submit the jobs
	qsub ${submission_script}
	rm ${submission_script}
done

