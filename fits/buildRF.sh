#!/bin/bash
#$ -N buildRapidFit
#$ -cwd
#$ -pe mpi 16
#$ -l h_vmem=1G
# Don't run if there are jobs in the queue
#$ -hold_jid j_*
# Hard runtime limit
#$ -l h_rt=00:05:00 
source RFjobconfig.sh
cd $RapidFitDir
make -j
