#!/bin/bash
cd ../ntuples
Dbranches=(KK_M Phi_angle cos_theta1 cos_theta2)
Nbranches=(BCON_KK_M Phi_angle cos_theta1 cos_theta2)
units=("MeV/#it{c}^{2}" none none none)
titles=("#it{m}(#it{K^{#plus}K^{#minus}})" "#it{#Phi}" "cos#it{#theta}_{1}" "cos#it{#theta}_{2}")
ranges=("--lower 900 --upper 1800" "--lower -3.141 --upper 3.141" "--lower -1 --upper 1" "--lower -1 --upper 1")
for i in `seq 0 3`
do
  ../bin/CompareBranch \
    --plot "../latex/figs/compare_acc_4D_${Dbranches[i]}" \
    --CDfile BsphiKK_MC_mvacut_acc_weights.root \
    --MCfile BsphiKK_Gen_5M_1800_mvaVars.root \
    --CDweight inverseacc \
    --CDcuts "KK_M<1800&&abs(KK_TRUEID)>500" \
    --CDbranch ${Nbranches[i]} \
    --MCbranch ${Dbranches[i]} \
    --unit ${units[i]} \
    --title ${titles[i]} \
    ${ranges[i]}
  ../bin/CompareBranchRatio \
    --plot "../latex/figs/compare_acc_ratio_4D_${Dbranches[i]}" \
    --Nfile BsphiKK_MC_mvacut_acc_weights.root \
    --Dfile BsphiKK_Gen_5M_1800_mvaVars.root \
    --Nweight inverseacc \
    --Ncuts "KK_M<1800&&abs(KK_TRUEID)>500" \
    --Nbranch ${Nbranches[i]} \
    --Dbranch ${Dbranches[i]} \
    --unit ${units[i]} \
    --title ${titles[i]} \
    ${ranges[i]}
done
exit 0

