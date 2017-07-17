#!/bin/bash
source eos.sh
outputfilename=mKKhist
figsdir=../../latex/figs/
bindir=../../bin
local_nTuples_dir=../../ntuples/
cd ../fits/results
make -C ../modules -j
# combinatorial background
{
fitting -f combinatorial_background.xml --calculateBackgroundCoefficients ## Angular part only
xrdcp --verbose --force LegendreMoments_1.root ${nTuples_dir}/LegendreMoments_combinatorial.root
rm -v LegendreMoments_1.root
mv -v sampled_LegendreMomentShape_1.root sampled_combinatorial.root
${bindir}/PlotAngAcc sampled_combinatorial.root
rename acceptance background *.pdf
}
# combinatorial background without the mva cut
{
fitting -f combinatorial_background_nomva.xml --calculateBackgroundCoefficients ## Angular part only
xrdcp --verbose --force LegendreMoments_1.root ${nTuples_dir}/LegendreMoments_combinatorial_nomva.root
rm -v LegendreMoments_1.root
mv -v sampled_LegendreMomentShape_1.root sampled_combinatorial_nomva.root
${bindir}/PlotAngAcc sampled_combinatorial_nomva.root
rename acceptance background_nomva *.pdf
}
# phi K* peaking background
fitting -f phikstar_background.xml --calculateBackgroundCoefficients ## Angular part only
xrdcp --verbose --force LegendreMoments_1.root ${nTuples_dir}/LegendreMoments_phikstar.root
rm -v LegendreMoments_1.root
mv -v sampled_LegendreMomentShape_1.root sampled_phikstar.root
${bindir}/PlotAngAcc sampled_phikstar.root
rename acceptance phikstar_background *.pdf
mv -v *background_*pdf ${figsdir}/
# Histogram for the mass-dependent part
${bindir}/PlotBranch  -F ${local_nTuples_dir}BsphiKK_data_mvacut.root \
                     -B BCON_KK_M_GeV \
                     -T "#it{m}(#it{K^{#plus}K^{#minus}})" \
                     -C "B_s0_LOKI_Mass>5500" \
                     -U "GeV/#it{c}^{2}" \
                     -l 0.980 \
                     -u 1.800 \
                     -b 41 \
                     -O $outputfilename \
                     --root
mv -v $outputfilename.pdf ${figsdir}/background_mKK_proj.pdf
xrdcp --verbose --force $outputfilename.root ${nTuples_dir}/background_mKKhist.root
rm -v $outputfilename.root
# Histogram for the mass-dependent part
${bindir}/PlotBranch  -F ${local_nTuples_dir}BsphiKK_data_mva.root \
                     -B BCON_KK_M_GeV \
                     -T "#it{m}(#it{K^{#plus}K^{#minus}})" \
                     -C "B_s0_LOKI_Mass>5500" \
                     -U "GeV/#it{c}^{2}" \
                     -l 0.980 \
                     -u 1.800 \
                     -b 41 \
                     -O $outputfilename \
                     --root
mv -v $outputfilename.pdf ${figsdir}/background_nomva_mKK_proj.pdf
xrdcp --verbose --force $outputfilename.root ${nTuples_dir}/background_nomva_mKKhist.root
rm -v $outputfilename.root
# Histogram for the mass-dependent part
${bindir}/PlotBranch  -F ${local_nTuples_dir}BdphiKst_MC_mvacut.root \
                     -B BCON_KK_M_GeV \
                     -T "#it{m}(#it{K^{#plus}K^{#minus}})" \
                     -U "GeV/#it{c}^{2}" \
                     -l 0.980 \
                     -u 1.800 \
                     -b 41 \
                     -O $outputfilename \
                     --root
mv -v $outputfilename.pdf ${figsdir}/phikstar_background_mKK_proj.pdf
xrdcp --verbose --force $outputfilename.root ${nTuples_dir}/phikstar_background_mKKhist.root
rm -v $outputfilename.root

