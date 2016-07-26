#!/bin/bash
source eos.sh
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_sideband)
for mode in ${modes[@]}
do
../bin/GetSelEff ${nTuples_dir}${mode}_nocut.root --save --output-file ../scripts/tables/SelEffs.csv  | tee log/${mode}_seleff.log
done
../bin/ExportResults ../scripts/tables/SelEffs.csv ../latex/results/SelEffs.tex
exit 0

