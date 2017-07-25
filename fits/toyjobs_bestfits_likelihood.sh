#!/bin/bash
# Usage: toyjobs_bestfits_likelihood.sh <folder1> <folder2> <folder3> <njobs> <nrepeats>
# This will create njobs jobs per model (i.e. 3*njobs in total)
# Each job will do nrepeats fits
source RFjobconfig.sh
currentdir=$(pwd)
if [ "$1" == "" ]
then
	echo "Please provide folder containing RapidFit outputXMLFiles"
	exit 0
else
	folder=${1}
fi
mainfolder=${currentdir}/results/datafits/bestfits_likelihood
mkdir -p ${mainfolder}
for i in $(seq 0 $((${4})))
do
	for Gfolder in ${1} ${2} ${3}
	do
		cd ${mainfolder}
		genxml=$(find ${currentdir}/${Gfolder} -type f -name "outputXMLFile*" | sort | tail -1 | sed 's/\.\///')
		submission_script="submit.sh"
		genname=$(echo "${Gfolder}" | sed "s/.*FitResult_//g" | sed "s/\///g")
		mkdir -p ${genname}
		cd ${genname}
		cat <<-EOF > ${submission_script}
		#!/bin/bash
		#$ -N "j_BF_LL_${genname}_${i}"
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
		export PATH=\$PATH:$currentdir/../RapidFit/bin:$currentdir/../bin
		for repeat in \`seq 0 $((${5}-1))\`
		do
			index=\$(($i*$nrepeats+\$repeat))
			# Generate the toys
			fitting -f ${genxml} --useUUID --saveOneDataSet toys_\${index}.root
			# Perform the fits
			for Ffolder in "${currentdir}/${1}" "${currentdir}/${2}" "${currentdir}/${3}"
			do
				cd ${mainfolder}/${genname}
				fitname=\$(echo "\${Ffolder}" | sed "s/.*FitResult_//g" | sed "s/\///g")
				mkdir -p \${fitname}
				cd \${fitname}
				cp -v \$(find \${Ffolder} -type f -name "outputXMLFile*" | sort | tail -1) fit_\${index}.xml
				sed -i "s/^.*TF1.*$//" fit_\${index}.xml
				sed -i "s/Foam/File/" fit_\${index}.xml
				sed -i "s/<NumberEvents.*$/<NumberEvents>1000000<\/NumberEvents><FileName>${mainfolder//\//\\/}\/${genname}\/toys_\${index}.root<\/FileName>\\\n\\\t<NTuplePath>dataNTuple<\/NTuplePath>/" fit_\${index}.xml
				outputdir=RapidFitOutput_\$(uuidgen)
				mkdir \${outputdir}
				logfile=\${outputdir}/RapidFitOutput.log
				fitting \${nThreadsFlag} -f fit_\${index}.xml --SendOutput \${outputdir} --MultiDimChi2 --ForceContinue 2>&1| tee \${logfile}
				mv fit_\${index}.xml \${outputdir}/fit.xml
			done
			cd ${mainfolder}/${genname}
			rm toys_\${index}.root
		done
		EOF
		# Submit the jobs
		qsub ${submission_script}
		rm ${submission_script}
	done
done

