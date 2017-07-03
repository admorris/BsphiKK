#!/bin/bash
source cuts.sh
cd ..
ntuple="ntuples/LbphiKp_MC_mvaVars.root"
categories=("abs(phiKpluspbarM-${Lbmass})>${Lbwindow}&&abs(phiKminuspM-${Lbmass})>${Lbwindow}" \
            "abs(phiKpluspbarM-${Lbmass})>${Lbwindow}&&abs(phiKminuspM-${Lbmass})<${Lbwindow}" \
            "abs(phiKpluspbarM-${Lbmass})<${Lbwindow}&&abs(phiKminuspM-${Lbmass})>${Lbwindow}" \
            "abs(phiKpluspbarM-${Lbmass})<${Lbwindow}&&abs(phiKminuspM-${Lbmass})<${Lbwindow}")
for category in ${categories[@]}
do
	echo -e "\n$category"
	bin/CutEff $ntuple "$category" "$LbphiKpveto"
done

