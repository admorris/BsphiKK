#!/bin/bash
source cuts.sh
cd ../ntuples
################################################################################
modes=($(ls *data*_mvaVars.root *sideband*_mvaVars.root *MC*_mvaVars.root | sed 's/_mvaVars\.root//'))
for mode in ${modes[@]}; do
  cutapplier ${mode}_mvaVars.root DecayTree "${phikstveto}&&${LbphiKpveto}&&${Lcphipveto}&&${DtoKaonsveto}&&${Dtophipiveto}" ${mode}_mvaVars_vetoes.root | tee log_${mode}.tmp &
done
wait
for mode in ${modes[@]}; do
  line=$(grep "accepted" log_${mode}.tmp)
  value=$(echo $line | sed -r 's/^accepted:\s*?(.*?)\s*\+\/-.*$/\1/')
  error=$(echo $line | sed -r 's/^accepted:.*?\+\/-\s*(.*?)\s*$/\1/')
  ../bin/UpdateResults ../scripts/tables/CutBasedSel.csv ${mode}_CBS_evts yield ${value} ${error}
done
rm -v log_*.tmp
../bin/ExportResults ../scripts/tables/CutBasedSel.csv ../latex/results/CutBasedSel.tex
exit 0

