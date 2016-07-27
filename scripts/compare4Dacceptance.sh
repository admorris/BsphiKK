#!/bin/bash
cd ../ntuples
branches=(KK_M Phi_angle cos_theta1 cos_theta2)
units=("MeV/#it{c}^{2}" none none none)
titles=("#it{m}(#it{K^{#plus}K^{#minus}})" "#it{#Phi}" "cos#it{#theta}_{1}" "cos#it{#theta}_{2}")
ranges=("-l 980 -u 1800" "-l -3.141 -u 3.141" "-l -1 -u 1" "-l -1 -u 1")
for i in `seq 0 3`
do
  plotname="../latex/figs/compare_acc_4D_${branches[i]}"
  ../bin/CompareBranchRatio \
    --plot $plotname \
    --Nfile BsphiKK_MC_mvacut_acc_weights.root \
    --Dfile BsphiKK_Gen_5M_1800_mvaVars.root \
    --Nweight inverseacc \
    --cuts "KK_M<1800" \
    --branch ${branches[i]} \
    --unit ${units[i]} \
    --title ${titles[i]} \
    ${ranges[i]}
done
exit 0

