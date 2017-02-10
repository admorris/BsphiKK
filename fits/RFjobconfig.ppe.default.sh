#!/bin/bash
# RapidFit location
export RapidFitDir=/Disk/ds-sopa-group/PPE/lhcb/users/admorris/RapidFit
export ParallelEnv="#$ -pe mpi 4"
# Load ROOT and TexLive
alias SetupEnvironment="source /cvmfs/lhcb.cern.ch/group_login.sh -c x86_64-centos7-gcc49-opt;SetupProject ROOT 6.06.02"
