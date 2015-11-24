#!/bin/bash
cd ../ntuples
../bin/AnnotateBranch \
    -F BsphiKK_data_mvacut.root \
    -B KK_M \
    -O ../latex/figs/mKKmvacut \
    -l 900 \
    -u 4100 \
    -b 64

../bin/AnnotateBranch \
    -F BsphiKK_data_1050_mvacut.root \
    -B KK_M \
    -O ../latex/figs/mKKmvacut_1050 \
    -l 1050 \
    -u 4100 \
    -b 61

../bin/PlotBranch \
    -F BsphiKK_data_1050_1800_mvacut.root \
    -B KK_M \
    -T "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" \
    -O ../latex/figs/mKKmvacut_1050_1800 \
    -l 1050 \
    -u 1800 \
    -b 15

../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_mvacut.root \
    -R ../ntuples/BsphiKK_data_mvacut.root \
    -O ../latex/figs/Bsmassfit_mvacut \
    --pulls

../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1050_mvacut.root \
    -R ../ntuples/BsphiKK_data_1050_mvacut.root \
    -O ../latex/figs/Bsmassfit_1050_mvacut 
    --pulls

../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1050_1800_mvacut.root \
    -R ../ntuples/BsphiKK_data_1050_1800_mvacut.root \
    -O ../latex/figs/Bsmassfit_1050_1800_mvacut \
    --pulls
