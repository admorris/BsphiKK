#!/bin/bash
#$ -N stageout
#$ -cwd
#$ -q staging
#$ -hold_jid j_*
# Hard runtime limit
#$ -l h_rt=01:00:00 
# Make the job resubmit itself if it runs out of time: rsync will start where it left off
#$ -r yes
#$ -notify
trap 'exit 99' sigusr1 sigusr2 sigterm
destination="/exports/csce/datastore/ph/groups/PPE/lhcb/users/admorris/BsphiKK/fits/"
fitfolders=(toystudies mcfits datafits)
for folder in ${fitfolders[@]}
do
	mkdir -p ${destination}/${folder}
	rsync -rlp ${folder} ${destination}
done

