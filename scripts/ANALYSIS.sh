#!/bin/bash
# A script to keep track of all the other scripts. Assumes the DaVinci output is available on the grid. If not, see the folder gridjobs.
# Please run all scripts from within the scripts folder
#./getoutputdata
#./merge.sh
./cut.sh
./addBranches.sh
./vetoes.sh
./sweight.sh
./trainmva.sh
./applymva.sh
./mvacut.sh
