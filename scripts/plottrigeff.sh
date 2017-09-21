#!/bin/bash
source eos.sh
tmpdir=/scratch/admorris/
mkdir -p $tmpdir
modes=(BsphiKK_MC)
for mode in ${modes[@]}
do
xrdcp ${nTuples_dir}${mode}_nocut.root ${tmpdir}${mode}_nocut.root
../bin/PlotTrigEff ${tmpdir}${mode}_nocut.root ../latex/figs/$mode
mv TrigEffPlots.root ../${mode}_TrigEffPlots.root
rm ${tmpdir}${mode}_nocut.root
done

