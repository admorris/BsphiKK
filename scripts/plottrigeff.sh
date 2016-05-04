#!/bin/bash
source eos.sh
modes=(BsphiKK_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/PlotTrigEff ${EOS_server}${EOS_nTuples_dir}${mode}_nocut.root
mv TrigEffPlots.root ../${mode}_TrigEffPlots.root
done
exit 0
