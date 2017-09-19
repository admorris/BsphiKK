#!/bin/bash
source eos.sh
outputfilename=mKKhist
figsdir=../../latex/figs/
bindir=../../bin
local_nTuples_dir=../../ntuples/
cd ../fits/results
make -C ../modules -j
bkgcuts=("1.8>BCON_KK_M_GeV\&\&B_s0_LOKI_Mass>5450\&\&5560>B_s0_LOKI_Mass" \
         "1.8>BCON_KK_M_GeV\&\&B_s0_LOKI_Mass>5560\&\&5670>B_s0_LOKI_Mass" \
         "1.8>BCON_KK_M_GeV\&\&B_s0_LOKI_Mass>5670" \
        )
for i in $(seq 0 2)
do
	# combinatorial background without the mva cut
	cat combinatorial_background_nomva.xml | sed "s/^\t*<CutString>.*$/<CutString>${bkgcuts[$i]}<\/CutString>/" > combinatorial_background_nomva_bin${i}.xml
	../../RapidFit/bin/fitting -f combinatorial_background_nomva_bin${i}.xml --calculateBackgroundCoefficients ## Angular part only
	rm -v LegendreMoments_1.root
	${bindir}/PlotAngAcc sampled_LegendreMomentShape_1.root
	rm sampled_LegendreMomentShape_1.root
	rename acceptance background_nomva_bin${i} *.pdf
	mv -v *background_*pdf ${figsdir}/
	# Histogram for the mass-dependent part
	${bindir}/PlotBranch  -F ${local_nTuples_dir}BsphiKK_data_mva.root \
	                      -B BCON_KK_M_GeV \
	                      -T "#it{m}(#it{K^{#plus}K^{#minus}})" \
	                      -C "${bkgcuts[$i]//\\&/&}" \
	                      -U "GeV/#it{c}^{2}" \
	                      -l 0.980 \
	                      -u 1.800 \
	                      -b 41 \
	                      -O $outputfilename
	mv -v $outputfilename.pdf ${figsdir}/background_nomva_mKK_proj_bin${i}.pdf
done
bkgcuts=("1.3>BCON_KK_M_GeV\&\&B_s0_LOKI_Mass>5500" \
         "BCON_KK_M_GeV>1.3\&\&1.5>BCON_KK_M_GeV\&\&B_s0_LOKI_Mass>5500" \
         "BCON_KK_M_GeV<1.5\&\&1.8>BCON_KK_M_GeV\&\&B_s0_LOKI_Mass>5500" \
        )
for i in $(seq 0 2)
do
	# combinatorial background without the mva cut
	cat combinatorial_background_nomva.xml | sed "s/^\t*<CutString>.*$/<CutString>${bkgcuts[$i]}<\/CutString>/" > combinatorial_background_nomva_bin${i}.xml
	../../RapidFit/bin/fitting -f combinatorial_background_nomva_bin${i}.xml --calculateBackgroundCoefficients ## Angular part only
	rm -v LegendreMoments_1.root
	${bindir}/PlotAngAcc sampled_LegendreMomentShape_1.root
	rm sampled_LegendreMomentShape_1.root
	rename acceptance background_nomva_mKKbin${i} *.pdf
	mv -v *background_*pdf ${figsdir}/
done

