#!/bin/bash
source eos.sh
#../bin/PlotBranch \
#    -F ${nTuples_dir}BsphiKK_MC_nocut_mvaVars.root \
#    -B Kplus0_IPCHI2_OWNPV \
#    -T "#it{K}^{#plus} #it{#chi}^{2}_{IP}" \
#    -O ../Kplus0_IPCHI2_OWNPV \
#    -l 0 \
#    -u 50 \
#    -b 50
#../bin/PlotBranch \
#    -F ${nTuples_dir}BsphiKK_MC_nocut_mvaVars.root \
#    -B Kminus0_IPCHI2_OWNPV \
#    -T "#it{K}^{#minus} #it{#chi}^{2}_{IP}" \
#    -O ../Kminus0_IPCHI2_OWNPV \
#    -l 0 \
#    -u 50 \
#    -b 50
#../bin/PlotBranch \
#    -F ${nTuples_dir}BsphiKK_MC_nocut_mvaVars.root \
#    -B Kplus_IPCHI2_OWNPV \
#    -T "#it{K}^{#plus} #it{#chi}^{2}_{IP}" \
#    -O ../Kplus_IPCHI2_OWNPV \
#    -l 0 \
#    -u 50 \
#    -b 50
#../bin/PlotBranch \
#    -F ${nTuples_dir}BsphiKK_MC_nocut_mvaVars.root \
#    -B Kminus_IPCHI2_OWNPV \
#    -T "#it{K}^{#minus} #it{#chi}^{2}_{IP}" \
#    -O ../Kminus_IPCHI2_OWNPV \
#    -l 0 \
#    -u 50 \
#    -b 50
../bin/PlotBranch \
    -F ${nTuples_dir}BsphiKK_MC_nocut_mvaVars.root \
    -B KK_IPCHI2_OWNPV \
    -T "#it{K}^{#plus}#it{K}^{#minus} #it{#chi}^{2}_{IP}" \
    -O ../KK_IPCHI2_OWNPV \
    -l 0 \
    -u 50 \
    -b 50
../bin/PlotBranch \
    -F ${nTuples_dir}BsphiKK_MC_nocut_mvaVars.root \
    -B phi_1020_IPCHI2_OWNPV \
    -T "#it{#phi} #it{#chi}^{2}_{IP}" \
    -O ../phi_1020_IPCHI2_OWNPV \
    -l 0 \
    -u 50 \
    -b 50
../bin/PlotBranch \
    -F ${nTuples_dir}BsphiKK_MC_nocut_mvaVars.root \
    -B B_s0_IPCHI2_OWNPV \
    -T "#it{B}^{0}_{#it{s}} #it{#chi}^{2}_{IP}" \
    -O ../B_s0_IPCHI2_OWNPV \
    -l 0 \
    -u 50 \
    -b 50


