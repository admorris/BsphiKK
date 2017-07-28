#!/bin/bash
# Please run this from the "fits" folder so it knows where the root macros are
folder=$1/$(ls $1 -tr | grep "RapidFitOutput_" | tail -1)
file=$(ls $folder/Global_Fit_Result*.root -tr | tail -1)
root -l -q -b "scripts/extractresults.C(\"$file\")" | grep -iv "^root" | grep -v "^Processing" | grep -v "^$"
root -l -q -b "scripts/extractfitfractions.C(\"$folder/fitFractions.root\")" | grep -iv "^root" | grep -v "^Processing" | grep -v "^$"

