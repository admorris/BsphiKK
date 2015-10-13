#!/bin/bash
# A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
# Please run all scripts from within the scripts folder
#./getoutputdata > log/getoutputdata.log
#./merge.sh > log/merge.log
./docuts.sh > log/docuts.log
./addBranches.sh > log/addBranches.log
./dovetoes.sh > log/dovetoes.log
./sweight.sh > log/sweight.log
./trainmva.sh > log/trainmva.log
./applymva.sh > log/applymva.log
./mvacut.sh > log/mvacut.log
