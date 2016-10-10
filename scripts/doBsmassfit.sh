#!/bin/bash
cd ../ntuples/
table=../scripts/tables/MassFits.csv
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut_final.root \
    -R BsphiKK_data_mvacut_final.root \
    -O ../latex/figs/Bsmassfit_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results Bsmassfitmvacut \
    --output-file ${table}
../bin/BsMassFit \
    -M BsphiKK_MC_1050_mvacut_final.root \
    -R BsphiKK_data_1050_mvacut_final.root \
    -O ../latex/figs/Bsmassfit_1050_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results BsmassfitmvacutA \
    --output-file ${table}
../bin/BsMassFit \
    -M BsphiKK_MC_1800_mvacut_final.root \
    -R BsphiKK_data_1800_mvacut_final.root \
    -O ../latex/figs/Bsmassfit_1800_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --draw-region 2 \
    --sweight \
    --save-results BsmassfitmvacutB \
    --output-file ${table}
#    --backgrounds f1420_Bs0_branches.root/HISTPDF BdphiKst_MC_mvacut_final.root LbphiKp_MC_mvacut_final.root \
#    --yields 0 0.02 475 \
#    --yopts flo flo flo \
#    --logy
../bin/BsMassFit \
    -M BsphiKK_MC_1050_1800_mvacut_final.root \
    -R BsphiKK_data_1050_1800_mvacut_final.root \
    -O ../latex/figs/Bsmassfit_1050_1800_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results BsmassfitmvacutAB \
    --output-file ${table}
../bin/ExportResults ${table} ../latex/results/MassFits.tex
exit 0

