#!/bin/bash
cd ../ntuples
../bin/FitLb --file BsphiKK_data_mvaVars_vetoes.root \
             --Sfile LbphiKp_MC_mvaVars_vetoes.root \
             --model "Crystal Ball + 2 Gaussians" \
             --upper 5700 \
             --lower 6000 \
             --cuts "BCON_KK_M<1800" \
             --plot ../latex/figs/LbphiKp_fit_for_yield \
             --pulls

../bin/FitLb --file BsphiKK_data_mvaVars_vetoes.root \
             --Sfile LbphiKp_MC_mvaVars_vetoes.root \
             --model "Crystal Ball + 2 Gaussians" \
             --upper 5700 \
             --lower 5550 \
             --cuts "BCON_KK_M<1800" \
             --plot ../latex/figs/LbphiKp_fit_for_yield_vetoes \
             --pulls
exit 0

