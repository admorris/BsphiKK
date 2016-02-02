#!/bin/bash
# A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
# Please run all scripts from within the scripts folder
#./getoutputdata | tee log/getoutputdata.log
#./merge.sh | tee log/merge.log
##./docuts.sh | tee log/docuts.log
#./addBranches.sh | tee log/addBranches.log
#./dovetoes.sh | tee log/dovetoes.log
#./showvetoes.sh | tee log/showvetoes.log
#./vetoeff.sh | tee log/vetoeff.log
#./seleff.sh
#./trigeff.sh
./sweight.sh | tee log/sweight.log
./plotMKK.sh
#./mvadataMCsWcomp.sh | tee log/mvadataMCsWcomp.log
#./trainmva.sh | tee log/trainmva.log
./applymva.sh | tee log/applymva.log
./mvaplots.sh | tee log/mvaplots.log
./mvadataMCsWcompOutput.sh | tee log/mvadataMCsWcompOutput.log
./mvacut.sh | tee log/mvacut.log
./mvacuteff.sh | tee log/mvacuteff.log
./plotmKK_mva.sh | tee log/plotmKK_mva.log
./plot4D.sh | tee log/plot4D.log
./doBsmassfit.sh | tee log/Bsmassfit.log
./angacc.sh | tee log/angacc.log
