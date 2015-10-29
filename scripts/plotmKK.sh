#!/bin/bash
cd ../ntuples
../bin/AnnotateMKK \
    -F BsphiKK_data_mvaVars_vetoes_Sweighted.root \
    -B KK_M \
    -W Nsig_sw \
    -O ../latex/figs/mKKsweight \
    -l 900 \
    -u 4000 \
    -b 62

../bin/PlotBranch \
    -F BsphiKK_data_mvaVars_vetoes.root \
    -B KK_M \
    -T "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" \
    -O ../latex/figs/mKK \
    -l 900 \
    -u 4000 \
    -b 62

../bin/AnnotateMKK \
    -F BsphiKK_data_1050_mvaVars_vetoes_Sweighted.root \
    -B KK_M \
    -W Nsig_sw \
    -O ../latex/figs/mKKsweight_1050 \
    -l 1050 \
    -u 4000 \
    -b 59

../bin/PlotBranch \
    -F BsphiKK_data_1050_mvaVars_vetoes.root \
    -B KK_M \
    -T "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" \
    -O ../latex/figs/mKK_1050 \
    -l 1050 \
    -u 4000 \
    -b 59

../bin/PlotBranch \
    -F BsphiKK_data_1050_1800_mvaVars_vetoes_Sweighted.root \
    -B KK_M \
    -W Nsig_sw \
    -T "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" \
    -O ../latex/figs/mKKsweight_1050_1800 \
    -l 1050 \
    -u 1800 \
    -b 15

../bin/PlotBranch \
    -F BsphiKK_data_1050_1800_mvaVars_vetoes.root \
    -B KK_M \
    -T "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" \
    -O ../latex/figs/mKK_1050_1800 \
    -l 1050 \
    -u 1800 \
    -b 15
