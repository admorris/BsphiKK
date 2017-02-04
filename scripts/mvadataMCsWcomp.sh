#!/bin/bash
cd ../ntuples
branches=(B_s0_ln_FDCHI2 B_s0_ln_IPCHI2 B_s0_ln_EVCHI2 B_s0_PT_fiveGeV B_s0_Eta minK_ln_IPCHI2 minK_PT_GeV)
Bs="#it{B}^{0}_{s}"
titles=("$Bs~ln#it{#chi}^{2}_{FD}" "$Bs~ln#it{#chi}^{2}_{IP}" "$Bs~ln#it{#chi}^{2}_{EV}" "$Bs~#it{p_{T}}~/~5" "$Bs~#eta" "min~#it{K}~ln#it{#chi}^{2}_{IP}" "min~#it{K}~#it{p_{T}}")
units=(none none none "GeV/#it{c}^{2}" none none "GeV/#it{c}^{2}")
ranges=("--lower 5 --upper 14" "--lower -6 --upper 4" "--lower -3 --upper 3" "--lower 0 --upper 8" "--lower 1 --upper 6" "--lower 0 --upper 10" "--lower 0 --upper 6" )
file="BsphiKK_data_mvaVars_vetoes_Sweighted.root"
MCfile="BsphiKK_MC_mvaVars_vetoes.root"
for i in `seq 0 6`
do
  plotname="../latex/figs/mvaVar$(echo $file | sed -r 's/.*data(.*)_mvaVars.*/\1/')_${branches[i]}"
  ../bin/CompareBranch \
       --CDfile $file \
       --MCfile $MCfile \
       --CDweight \"Nsig_sw\" \
       --MCbranch \"${branches[i]}\" \
       --CDbranch \"${branches[i]}\" \
       --title \"${titles[i]}\" \
       --unit \"${units[i]}\" \
       --plot \"$plotname\" \
       ${ranges[i]}
done
exit 0

