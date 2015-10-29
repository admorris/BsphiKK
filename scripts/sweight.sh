#!/bin/bash
cd ../ntuples/
modes=(BsphiKK_data BsphiKK_MC BsphiKK_sideband)
for mode in ${modes[@]}; do
cutapplier ${mode}_mvaVars_vetoes.root DecayTree "KK_M>1050" ${mode}_1050_mvaVars_vetoes.root
cutapplier ${mode}_mvaVars_vetoes.root DecayTree "KK_M>1050&&KK_M<1800" ${mode}_1050_1800_mvaVars_vetoes.root
done
cd ../fits/
../bin/BsMassFit -M ../ntuples/BsphiKK_MC_mvaVars_vetoes.root -R ../ntuples/BsphiKK_data_mvaVars_vetoes.root --sweight -O ../latex/figs/initialmassfit --pulls
../bin/BsMassFit -M ../ntuples/BsphiKK_MC_1050_mvaVars_vetoes.root -R ../ntuples/BsphiKK_data_1050_mvaVars_vetoes.root --sweight -O ../latex/figs/initialmassfit1050 --pulls
../bin/BsMassFit -M ../ntuples/BsphiKK_MC_1050_1800_mvaVars_vetoes.root -R ../ntuples/BsphiKK_data_1050_1800_mvaVars_vetoes.root --sweight -O ../latex/figs/initialmassfit10501800 --pulls --draw-region
