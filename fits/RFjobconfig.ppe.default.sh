#!/bin/bash
export ParallelEnv="#$ -pe mpi 4"
export SetupEnvironment="source /cvmfs/lhcb.cern.ch/group_login.sh -c x86_64-centos7-gcc49-opt\n. SetupProject.sh ROOT 6.06.02"

