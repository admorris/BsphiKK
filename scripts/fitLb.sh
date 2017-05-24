#!/bin/bash
source cuts.sh
cd ../ntuples
table=../scripts/tables/MassFits.csv
Lbfitwindow=60
cutapplier BsphiKK_data_mvaVars.root "DecayTree" "abs(phiKpM-${Lbmass})<${Lbfitwindow}&&BCON_KK_M<1800" LbphiKp_data_mvaVars.root
../bin/FitLb --file LbphiKp_data_mvaVars.root \
             --Sfile LbphiKp_MC_mvaVars.root \
             --model "Crystal Ball + 2 Gaussians" \
             --upper $(echo "${Lbmass}+${Lbfitwindow}" | bc -l) \
             --lower $(echo "${Lbmass}-${Lbfitwindow}" | bc -l) \
             --plot ../latex/figs/LbphiKp_fit_for_yield \
             --bins $(echo "${Lbfitwindow}/2" | bc -l | sed 's/\..*//') \
             --sweight \
             --save-results LbphiKpdatafit \
             --output-file ${table} \
             --pulls

#../bin/FitLb --file LbphiKp_data_mvaVars.root \
#             --Sfile LbphiKp_MC_mvaVars.root \
#             --model "Crystal Ball + 2 Gaussians" \
#             --upper $(echo "${Lbmass}+${Lbwindow}" | bc -l) \
#             --lower $(echo "${Lbmass}-${Lbwindow}" | bc -l) \
#             --cuts "${LbphiKpveto}" \
#             --plot ../latex/figs/LbphiKp_fit_for_yield_vetoes \
#             --bins $(echo "${Lbwindow}/2" | bc -l | sed 's/\..*//') \
#             --pulls


