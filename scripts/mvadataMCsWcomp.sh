#!/bin/bash
cd ../ntuples
branches=(B_s0_ln_FDCHI2 B_s0_ln_IPCHI2 B_s0_ln_EVCHI2 B_s0_PT_fiveGeV B_s0_Eta minK_ln_IPCHI2 minK_PT_GeV)
Bs="#it{B}^{0}_{s}"
titles=("$Bs~ln#it{#chi}^{2}_{FD}" "$Bs~ln#it{#chi}^{2}_{IP}" "$Bs~ln#it{#chi}^{2}_{EV}" "$Bs~#it{p_{T}}~/~5" "$Bs~#eta" "min~#it{K}~ln#it{#chi}^{2}_{IP}" "min~#it{K}~#it{p_{T}}")
units=(none none none "GeV/#it{c}^{2}" none none "GeV/#it{c}^{2}")
ranges=("-l 5 -u 14" "-l -6 -u 4" "-l -3 -u 3" "-l 0 -u 8" "-l 1 -u 6" "-l 0 -u 10" "-l 0 -u 6" )
for file in $(ls *mvaVars_vetoes_Sweighted.root)
do
  MCfile=$(echo $file | sed 's/data/MC/' | sed 's/_Sweighted//')
  for i in `seq 0 6`
  do
    plotname="../latex/figs/mvaVar$(echo $file | sed -r 's/.*data(.*)_mvaVars.*/\1/')_${branches[i]}"
    ../bin/CompareBranch \
         -R $file \
         -M $MCfile \
         -W \"Nsig_sw\" \
         -B \"${branches[i]}\" \
         -T \"${titles[i]}\" \
         -U \"${units[i]}\" \
         -O \"$plotname\" \
         ${ranges[i]}
  done
done
exit 0



