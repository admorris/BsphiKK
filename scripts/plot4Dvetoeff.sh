#!/bin/bash
cd ../ntuples

for mode in "BsphiKK_data" "BsphiKK_MC" "Bsphiphi_MC"
do
../bin/CompareBranchRatio \
    --Dfile ${mode}_mvaVars.root \
    --Nfile ${mode}_mvaVars_vetoes.root \
    --Dcuts "BCON_KK_M<1800" \
    --Ncuts "BCON_KK_M<1800" \
    --Dbranch BCON_KK_M_GeV \
    --Nbranch BCON_KK_M_GeV \
    --title "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" \
    --plot ../latex/figs/vetoeff_${mode}_mKK \
    --unit "GeV/#it{c}^{2}" \
    --lower 0.980 \
    --upper 1.800 \
    --bins 41

../bin/CompareBranchRatio \
    --Dfile ${mode}_mvaVars.root \
    --Nfile ${mode}_mvaVars_vetoes.root \
    --Dcuts "BCON_KK_M<1800" \
    --Ncuts "BCON_KK_M<1800" \
    --Dbranch Phi_angle \
    --Nbranch Phi_angle \
    --title "#it{#Phi}" \
    --plot ../latex/figs/vetoeff_${mode}_Phi_angle \
    --unit "none" \
    --lower -3.142 \
    --upper 3.142 \
    --bins 15

../bin/CompareBranchRatio \
    --Dfile ${mode}_mvaVars.root \
    --Nfile ${mode}_mvaVars_vetoes.root \
    --Dcuts "BCON_KK_M<1800" \
    --Ncuts "BCON_KK_M<1800" \
    --Dbranch cos_theta1 \
    --Nbranch cos_theta1 \
    --title "cos(#it{#theta_{1}})" \
    --plot ../latex/figs/vetoeff_${mode}_cos_theta1 \
    --unit "none" \
    --lower -1 \
    --upper 1 \
    --bins 15
    
../bin/CompareBranchRatio \
    --Dfile ${mode}_mvaVars.root \
    --Nfile ${mode}_mvaVars_vetoes.root \
    --Dcuts "BCON_KK_M<1800" \
    --Ncuts "BCON_KK_M<1800" \
    --Dbranch cos_theta2 \
    --Nbranch cos_theta2 \
    --title "cos(#it{#theta_{2}})" \
    --plot ../latex/figs/vetoeff_${mode}_cos_theta2 \
    --unit "none" \
    --lower -1 \
    --upper 1 \
    --bins 15
done

