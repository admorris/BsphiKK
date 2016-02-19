#!/bin/bash
## A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
## Please run all scripts from within the scripts folder
##
## Get DaVinci output
./getoutputdata | tee log/getoutputdata.log
./merge.sh | tee log/merge.log
## Cut-based selection
./docuts.sh | tee log/docuts.log
./addBranches.sh | tee log/addBranches.log
./dovetoes.sh | tee log/dovetoes.log
./showvetoes.sh | tee log/showvetoes.log
## Efficiencies
./vetoeff.sh | tee log/vetoeff.log
./seleff.sh
./trigeff.sh
## Preliminary m(KK) plots
./sweight.sh | tee log/sweight.log
./plotMKK.sh
## Multivariate selection
./mvadataMCsWcomp.sh | tee log/mvadataMCsWcomp.log
./trainmva.sh | tee log/trainmva.log
./applymva.sh | tee log/applymva.log
./mvaplots.sh | tee log/mvaplots.log
./mvadataMCsWcompOutput.sh | tee log/mvadataMCsWcompOutput.log
./mvacut.sh | tee log/mvacut.log
./mvacuteff.sh | tee log/mvacuteff.log
## Plot fully-selected m(KK) and helicity angles
./plotmKK_mva.sh | tee log/plotmKK_mva.log
./plot4D.sh | tee log/plot4D.log
## Get input parameters for angular fit
./doBsmassfit.sh | tee log/Bsmassfit.log
./angacc.sh | tee log/angacc.log
./bkgparam.sh | tee log/bkgparam.log
