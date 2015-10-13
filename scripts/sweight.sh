#!/bin/bash
cd ../ntuples/
modes=(BsphiKK_data BsphiKK_MC)
for mode in ${modes[@]}; do
cutapplier ${mode}_mvaVars_vetoes.root DecayTree "KK_M>1050" ${mode}_1050_mvaVars_vetoes.root
cutapplier ${mode}_mvaVars_vetoes.root DecayTree "KK_M>1050&&KK_M<1800" ${mode}_1050_1800_mvaVars_vetoes.root
done
../bin/BsMassFit BsphiKK_MC_mvaVars_vetoes.root BsphiKK_data_mvaVars_vetoes.root "Crystal Ball + 2 Gaussians" sweight
../bin/BsMassFit BsphiKK_MC_1050_mvaVars_vetoes.root BsphiKK_data_1050_mvaVars_vetoes.root "Crystal Ball + 2 Gaussians" sweight
../bin/BsMassFit BsphiKK_MC_1050_1800_mvaVars_vetoes.root BsphiKK_data_1050_1800_mvaVars_vetoes.root "Crystal Ball + 2 Gaussians" sweight
return 0
