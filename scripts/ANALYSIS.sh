#!/bin/bash
# A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
# Please run all scripts from within the scripts folder
#./getoutputdata | tee log/getoutputdata.log
#./merge.sh | tee log/merge.log
#./docuts.sh | tee log/docuts.log
#./addBranches.sh | tee log/addBranches.log
#./dovetoes.sh | tee log/dovetoes.log
#./sweight.sh | tee log/sweight.log
./trainmva.sh | tee log/trainmva.log
./applymva.sh | tee log/applymva.log
./mvacut.sh | tee log/mvacut.log
