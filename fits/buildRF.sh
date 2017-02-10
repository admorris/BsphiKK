#!/bin/bash
source RFjobconfig.sh
currentdir=$(pwd)
submission_script=tmpbuildjob.sh
cat <<-EOF > ${submission_script}
	#!/bin/bash
	#$ -N buildRapidFit
	#$ -cwd
	$ParallelEnv
	#$ -l h_vmem=1G
	# Don't run if there are jobs in the queue
	#$ -hold_jid j_*
	# Hard runtime limit
	#$ -l h_rt=00:10:00
	source $currentdir/RFjobconfig.sh
	$SetupEnvironment
	cd \$RapidFitDir
	make -j
	EOF
# Submit the job
qsub ${submission_script}
rm ${submission_script}

