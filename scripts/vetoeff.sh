#!/bin/bash
modes=(BsphiKK_MC)
for mode in ${modes[@]}
do
../bin/GetVetoEff ../ntuples/${mode}_mvaVars.root | tee log/${mode}_vetoeff.log
done
###########################################################
exit 0
