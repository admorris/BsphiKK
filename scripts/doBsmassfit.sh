#!/bin/bash
cd ../ntuples/
table=../scripts/tables/MassFits.csv
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut.root \
    -O ../latex/figs/Bsmassfit_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results Bsmassfitmvacut \
    --output-file ${table} \
ranges=(1080 1200 1350 1450 1550 1650 1800)
for range in ${ranges[@]}
do
cutapplier BsphiKK_data_mvacut.root DecayTree "BCON_KK_M<$range" BsphiKK_data_mvacut_$range.root
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut_$range.root \
    -O ../latex/figs/Bsmassfit_mvacut_$range \
    -N B_s0_LOKI_Mass \
    --pulls \
    --sweight
done
cutapplier BsphiKK_data_mvacut.root DecayTree "abs(BCON_KK_M-1019.461)<15" BsphiKK_data_mvacut_phirange.root
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut_phirange.root \
    -O ../latex/figs/Bsmassfit_mvacut_phirange \
    -N B_s0_LOKI_Mass \
    --pulls \
    --sweight
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut.root \
    -C "BCON_KK_M>1050" \
    -O ../latex/figs/Bsmassfit_1050_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results BsmassfitmvacutA \
    --output-file ${table}
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut.root \
    -C "BCON_KK_M<1800" \
    -O ../latex/figs/Bsmassfit_1800_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls \
    --draw-region 2 \
    --save-results BsmassfitmvacutB \
    --output-file ${table}
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut.root \
    -C "BCON_KK_M<1800" \
    -O ../latex/figs/Bsmassfit_1800_mvacut_pkgbkgs \
    -N B_s0_LOKI_Mass \
    --pulls \
    --draw-region 2 \
    --save-results BsmassfitmvacutpkgbkgsB \
    --output-file ${table} \
    --backgrounds f1420_Bs0_branches.root/HISTPDF BdphiKst_MC_mvacut.root LbphiKp_MC_mvacut.root \
    --yields 0 0.02 475 \
    --yopts flo flo flo \
    --logy
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut.root \
    -C "BCON_KK_M>1050&&BCON_KK_M<1800" \
    -O ../latex/figs/Bsmassfit_1050_1800_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls \
    --save-results BsmassfitmvacutAB \
    --output-file ${table}
../bin/ExportResults ${table} ../latex/results/MassFits.tex
exit 0

