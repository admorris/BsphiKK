#!/bin/bash
table=../scripts/tables/mKKcutResults.csv
cd ../ntuples/
modes=(BsphiKK_data BsphiKK_MC BsphiKK_sideband)
for mode in ${modes[@]}; do
  cutapplier ${mode}_mvaVars_vetoes.root DecayTree "KK_M>1050" ${mode}_1050_mvaVars_vetoes.root | tee log_${mode}_1050.tmp &
  cutapplier ${mode}_mvaVars_vetoes.root DecayTree "KK_M<1800" ${mode}_1800_mvaVars_vetoes.root | tee log_${mode}_1800.tmp &
  cutapplier ${mode}_mvaVars_vetoes.root DecayTree "KK_M>1050&&KK_M<1800" ${mode}_1050_1800_mvaVars_vetoes.root | tee log_${mode}_1050_1800.tmp &
done
wait
for mode in ${modes[@]}; do
  for label in 1050 1800 1050_1800; do
    line=$(grep "accepted" log_${mode}_${label}.tmp)
    value=$(echo $line | sed -r 's/^accepted:\s*?(.*?)\s*\+\/-.*$/\1/')
    error=$(echo $line | sed -r 's/^accepted:.*?\+\/-\s*(.*?)\s*$/\1/')
    ../bin/UpdateResults ${table} ${mode}_$(echo ${label} | sed 's/1050/A/' | sed 's/1800/B/') yield ${value} ${error}
  done
rm -v log_${mode}*.tmp
done
cd ../fits/
../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_mvaVars_vetoes.root \
    -R ../ntuples/BsphiKK_data_mvaVars_vetoes.root \
    -O ../latex/figs/initialmassfit \
    -N B_s0_LOKI_Mass \
    --sweight \
    --pulls
../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1050_mvaVars_vetoes.root \
    -R ../ntuples/BsphiKK_data_1050_mvaVars_vetoes.root \
    -O ../latex/figs/initialmassfit1050 \
    -N B_s0_LOKI_Mass \
    --sweight \
    --pulls
../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1800_mvaVars_vetoes.root \
    -R ../ntuples/BsphiKK_data_1800_mvaVars_vetoes.root \
    -O ../latex/figs/initialmassfit1800 \
    -N B_s0_LOKI_Mass \
    --sweight \
    --pulls
../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1050_1800_mvaVars_vetoes.root \
    -R ../ntuples/BsphiKK_data_1050_1800_mvaVars_vetoes.root \
    -O ../latex/figs/initialmassfit10501800 \
    -N B_s0_LOKI_Mass \
    --sweight \
    --pulls

