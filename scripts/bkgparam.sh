#!/bin/bash
#table=../scripts/tables/MassFits.csv
cd ../fits
fitting -f background.xml --calculateBackgroundCoefficients ## Angular part only
mv LegendreMoments.root LegendreMoments_combinatorial.root
mv sampled_LegendreMomentShape.root sampled_combinatorial.root
../bin/PlotAngAcc sampled_combinatorial.root
rename acceptance background *.pdf
mv -v background_*pdf ../latex/figs/
# Histogram for the mass-dependent part
../bin/PlotBranch  -F ../ntuples/BsphiKK_data_mvacut.root \
                   -B BCON_KK_M \
                   -T "#it{m}(#it{K^{#plus}K^{#minus}})" \
                   -C "abs(B_s0_LOKI_Mass-5366.77)>100" \
                   -U "MeV/#it{c}^{2}" \
                   -l 980 \
                   -u 1800 \
                   -b 41 \
                   -O ../latex/figs/background_mKK_proj \
                   --root

#../bin/ExportResults ${table} ../latex/results/MassFits.tex
exit 0

