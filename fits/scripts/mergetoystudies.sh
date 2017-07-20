#!/bin/bash
cd $1
for prefix in "acceptance" "toy";
do
	for filepattern in "fitFractions" "fitFractions_10_MeV" "fitFractions_15_MeV" "fitFractions_20_MeV" "fitFractions_25_MeV" "Global_Fit_Result*"
	do
		hadd -f ${prefix}_${filepattern/\*/}.root $(find ${prefix}job*/ -name "${filepattern}.root")
	done
done
cd -

