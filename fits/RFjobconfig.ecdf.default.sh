#!/bin/bash
# RapidFit location
export RapidFitDir=~/RapidFit
export ParallelEnv="#$ -pe sharedmem 4"
# Load ROOT and TexLive
function SetupEnvironment()
{
	. /etc/profile.d/modules.sh
	module load root
	module load igmm/apps/texlive
}

