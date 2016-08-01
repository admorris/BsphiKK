#!/bin/bash
branch='mlp'
unit='none'
title='MLP~output'
range='--lower 0 --upper 1'

cd ../ntuples
for file in $(ls *mva_Sweighted.root)
do
  MCfile=$(echo $file | sed 's/data/MC/' | sed 's/_Sweighted//')
  plotname="../latex/figs/mvaOutput$(echo $file | sed -r 's/.*data(.*)_mva.*/\1/')_${branch}"
  ../bin/CompareBranch \
       --CDfile $file \
       --MCfile $MCfile \
       --CDweight \"Nsig_sw\" \
       --MCbranch \"${branch}\" \
       --CDbranch \"${branch}\" \
       --title \"${title}\" \
       --unit \"${unit}\" \
       --plot \"$plotname\" \
       ${range} \
       --bins 100
done
exit 0

