#!/bin/bash
# RapidFit location
export RapidFitDir=/Disk/ds-sopa-group/PPE/lhcb/users/admorris/RapidFit
export ParallelEnv="#$ -pe mpi 4"
# Load ROOT and TexLive
function SetupEnvironment()
{
	source /cvmfs/lhcb.cern.ch/group_login.sh -c x86_64-centos7-gcc49-opt
	. SetupProject.sh ROOT 6.06.02
}
