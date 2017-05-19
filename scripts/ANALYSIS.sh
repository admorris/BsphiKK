#!/bin/bash
## A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
## Please run all scripts from within the scripts folder
##
## Get DaVinci output
#./getoutputdata 2>&1| tee log/getoutputdata.log
#./merge.sh 2>&1| tee log/merge.log
#./sideband.sh 2>&1| tee log/sideband.log
./SELECTION.sh
./EFFICIENCIES.sh
./SELECTION_PLOTS.sh
## Get input parameters for angular fit
./fitLb.sh 2>&1| tee log/fitLb.log
./doBsmassfit.sh 2>&1| tee log/Bsmassfit.log
./plotmoments.sh 2>&1| tee log/plotmoments.log
./angacc.sh 2>&1| tee log/angacc.log
./bkgparam.sh 2>&1| tee log/bkgparam.log

