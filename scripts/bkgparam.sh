#!/bin/bash
source eos.sh
#table=../scripts/tables/MassFits.csv
outputfilename=mKKhist
cd ../fits
# combinatorial background
fitting -f combinatorial_background.xml --calculateBackgroundCoefficients ## Angular part only
mv -v LegendreMoments.root ${nTuples_dir}/LegendreMoments_combinatorial.root
mv -v sampled_LegendreMomentShape.root sampled_combinatorial.root
../bin/PlotAngAcc sampled_combinatorial.root
rename acceptance background *.pdf
# phi K* peaking background
fitting -f phikstar_background.xml --calculateBackgroundCoefficients ## Angular part only
mv -v LegendreMoments.root ${nTuples_dir}/LegendreMoments_phikstar.root
mv -v sampled_LegendreMomentShape.root sampled_phikstar.root
../bin/PlotAngAcc sampled_phikstar.root
rename acceptance phikstar_background *.pdf
mv -v *background_*pdf ../latex/figs/
# Histogram for the mass-dependent part
../bin/PlotBranch  -F ../ntuples/BsphiKK_data_mvacut.root \
                   -B BCON_KK_M_GeV \
                   -T "#it{m}(#it{K^{#plus}K^{#minus}})" \
                   -C "abs(B_s0_LOKI_Mass-5366.77)>100" \
                   -U "GeV/#it{c}^{2}" \
                   -l 0.980 \
                   -u 1.800 \
                   -b 41 \
                   -O $outputfilename \
                   --root
mv -v $outputfilename.pdf ../latex/figs/background_mKK_proj.pdf
mv -v $outputfilename.root ${nTuples_dir}/background_mKKhist.root
# Histogram for the mass-dependent part
../bin/PlotBranch  -F ../ntuples/BdphiKst_MC_mvacut.root \
                   -B BCON_KK_M_GeV \
                   -T "#it{m}(#it{K^{#plus}K^{#minus}})" \
                   -C "abs(B_s0_LOKI_Mass-5366.77)>100" \
                   -U "GeV/#it{c}^{2}" \
                   -l 0.980 \
                   -u 1.800 \
                   -b 41 \
                   -O $outputfilename \
                   --root
mv -v $outputfilename.pdf ../latex/figs/phikstar_background_mKK_proj.pdf
mv -v $outputfilename.root ${nTuples_dir}/phikstar_background_mKKhist.root
#../bin/ExportResults ${table} ../latex/results/MassFits.tex
exit 0

