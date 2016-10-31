#!/bin/bash
cd ../ntuples/
table=../scripts/tables/MassFits.csv
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut_final.root \
    -R BsphiKK_data_mvacut_final.root \
    -O ../latex/figs/Bsmassfit_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results Bsmassfitmvacut_final \
    --output-file ${table} \
    --sweight
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut_final.root \
    -R BsphiKK_data_mvacut_final.root \
    -C "BCON_KK_M>1050" \
    -O ../latex/figs/Bsmassfit_1050_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results Bsmassfitmvacut_finalA \
    --output-file ${table}
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut_final.root \
    -R BsphiKK_data_mvacut_final.root \
    -C "BCON_KK_M<1800" \
    -O ../latex/figs/Bsmassfit_1800_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --draw-region 2 \
    --save-results Bsmassfitmvacut_finalB \
    --output-file ${table}
#    --sweight \ # to sweight, need to use cutapplier and make a new file :/
#    --backgrounds f1420_Bs0_branches.root/HISTPDF BdphiKst_MC_mvacut_final.root LbphiKp_MC_mvacut_final.root \
#    --yields 0 0.02 475 \
#    --yopts flo flo flo \
#    --logy
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut_final.root \
    -R BsphiKK_data_mvacut_final.root \
    -C "BCON_KK_M>1050&&BCON_KK_M<1800" \
    -O ../latex/figs/Bsmassfit_1050_1800_mvacut_final \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results Bsmassfitmvacut_finalAB \
    --output-file ${table}
../bin/ExportResults ${table} ../latex/results/MassFits.tex
exit 0

