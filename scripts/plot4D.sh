#!/bin/bash
cd ../ntuples
../bin/PlotBranch \
    -F BsphiKK_data_mvacut_Sweighted.root \
    -C "BCON_KK_M<1800" \
    -W "Nsig_sw" \
    -B KK_M \
    -T "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" \
    -O ../latex/figs/mKK_1800 \
    -l 950 \
    -u 1800 \
    -b 34

../bin/PlotBranch \
    -F BsphiKK_data_mvacut_Sweighted.root \
    -C "BCON_KK_M<1800" \
    -W "Nsig_sw" \
    -B Phi_angle \
    -T "#it{#Phi}" \
    -O ../latex/figs/Phi_angle_1800 \
    -U "none" \
    -l -3.142 \
    -u 3.142 \
    -b 15

../bin/PlotBranch \
    -F BsphiKK_data_mvacut_Sweighted.root \
    -C "BCON_KK_M<1800" \
    -W "Nsig_sw" \
    -B cos_theta1 \
    -T "cos(#it{#theta_{1}})" \
    -O ../latex/figs/cos_theta1_1800 \
    -U "none" \
    -l -1 \
    -u 1 \
    -b 15
    
../bin/PlotBranch \
    -F BsphiKK_data_mvacut_Sweighted.root \
    -C "BCON_KK_M<1800" \
    -W "Nsig_sw" \
    -B cos_theta2 \
    -T "cos(#it{#theta_{2}})" \
    -O ../latex/figs/cos_theta2_1800 \
    -U "none" \
    -l -1 \
    -u 1 \
    -b 15
exit 0

