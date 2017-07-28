#!/bin/bash
# Provide a folder containing one or more "FitResult" folders
# It will use the most recent fit from each
# Please run this from the "fits" folder so it knows where the root macros are
for FRfolder in $(find $1 -type d -name "FitResult*")
do
	echo $FRfolder
	./scripts/resultstable.sh $FRfolder
	read -n 1
done

