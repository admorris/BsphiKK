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
mkdir -p $folder/plots
for Gmodel in $models
do
	for Fmodel in $models
	do
		cd $folder/$Gmodel/$Fmodel
		mergedname="Global_Fit_Result.root"
		hadd -f $mergedname RapidFitOutput_*/Global_Fit_Result_*.root
		root -q -b -l "$cwd/scripts/fitbestfitNLL.C(\"$mergedname\", $(getdatalikelihood $Fmodel))"
		mv -v NLL.pdf "$folder/plots/gen_${Gmodel}_fit_${Fmodel}.pdf"
	done
done

