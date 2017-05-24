#!/bin/bash
branch='mlp'
unit='none'
title='MLP~output'
range='--lower 0.8 --upper 1'
MCfile="BsphiKK_MC_mva.root"
CDfile="BsphiKK_data_mva.root"
plotname="../latex/figs/mvaOutput_${branch}"
cd ../ntuples
../bin/BsMassFit \
    -M $MCfile \
    -R $CDfile \
    -O tmp \
    -N B_s0_LOKI_Mass \
    --sweight
rm tmp*
CDfile="BsphiKK_data_mva_Sweighted.root"
../bin/CompareBranch \
     --MCfile $MCfile \
     --CDfile $CDfile \
     --CDweight Nsig_sw \
     --MCbranch ${branch} \
     --CDbranch ${branch} \
     --title ${title} \
     --unit ${unit} \
     --plot $plotname \
     ${range} \
     --bins 20


