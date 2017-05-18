#!/bin/bash
modes=(BsphiKK_MC)
for mode in ${modes[@]}
do
../bin/GetVetoEff ../ntuples/${mode}_mvaVars.root --save --output-file ../scripts/tables/VetoEffs.csv 2>&1| tee log/${mode}_vetoeff.log
done
../bin/ExportResults ../scripts/tables/VetoEffs.csv ../latex/results/VetoEffs.tex
###########################################################


