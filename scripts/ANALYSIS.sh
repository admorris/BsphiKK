#!/bin/bash
## A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
## Please run all scripts from within the scripts folder
##
## Get DaVinci output
#./getoutputdata | tee log/getoutputdata.log
#./merge.sh | tee log/merge.log
#./sideband.sh | tee log/sideband.log
./SELECTION.sh
./EFFICIENCIES.sh
./SELECTION_PLOTS.sh
## Get input parameters for angular fit
./doBsmassfit.sh | tee log/Bsmassfit.log
./plotmoments.sh | tee log/plotmoments.log
./angacc.sh | tee log/angacc.log


