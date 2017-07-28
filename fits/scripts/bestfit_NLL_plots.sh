#!/bin/bash
# Please run this from the "fits" folder so it knows where the PlotBranch binary is
cwd=$(pwd)
function getdatalikelihood()
{
	FRfolder=$(find $cwd/results/datafits/*_extra -type d -name "FitResult_$1")
	echo $(grep "NLL:" $(ls $FRfolder/RapidFitOutput-*.log -tr | tail -1) | sed 's/^.*NLL:\s*//g')
}
folder=$cwd/results/datafits/bestfits_likelihood
models=$(ls $folder | grep "nonres_fzero980_phi1020_ftwop1525")
for Gmodel in $models
do
	for Fmodel in $models
	do
		cd $folder/$Gmodel/$Fmodel
		mergedname="Global_Fit_Result.root"
#		hadd -f $mergedname RapidFitOutput_*/Global_Fit_Result_*.root
#		$cwd/../bin/PlotBranch -F Global_Fit_Result.root -B NLL -T "NLL" -U "" -O "NLL" -C "Fit_Status>1" -l 2800 -u 4000 -b 12 --lineat $(getdatalikelihood $Fmodel)
		root -q -b -l "$cwd/scripts/fitbestfitNLL.C(\"$mergedname\", $(getdatalikelihood $Fmodel))"
	done
done

