#!/bin/bash
source eos.sh
modes=(BsphiKK_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/PlotTrigEff ${nTuples_dir}${mode}_nocut.root ../latex/figs/$mode
mv TrigEffPlots.root ../${mode}_TrigEffPlots.root
done

