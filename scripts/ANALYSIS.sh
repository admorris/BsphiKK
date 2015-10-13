#!/bin/bash
# A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
# Please run all scripts from within the scripts folder
#./getoutputdata > log/getoutputdata.log
#./merge.sh > log/merge.log
./cut.sh > log/cut.log
./addBranches.sh > log/addBranches.log
./vetoes.sh > log/vetoes.log
./sweight.sh > log/sweight.log
./trainmva.sh > log/trainmva.log
./applymva.sh > log/applymva.log
./mvacut.sh > log/mvacut.log
