#!/bin/bash
branch='mlp'
unit='none'
title='MLP~output'
range='--lower 0 --upper 1'
CDfile="BsphiKK_MC_mva.root"
MCfile="BsphiKK_data_mva.root"
plotname="../latex/figs/mvaOutput_${branch}"
cd ../ntuples
../bin/BsMassFit \
    -M $CDfile \
    -R $MCfile \
    -O tmp \
    -N B_s0_LOKI_Mass \
    --sweight
rm tmp*
CDfile="BsphiKK_data_mva_Sweighted.root"
../bin/CompareBranch \
     --CDfile $CDfile \
     --MCfile $MCfile \
     --CDweight Nsig_sw \
     --MCbranch ${branch} \
     --CDbranch ${branch} \
     --title ${title} \
     --unit ${unit} \
     --plot $plotname \
     ${range} \
     --bins 100


