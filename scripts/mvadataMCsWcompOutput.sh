#!/bin/bash
branch='mlp'
unit='none'
title='MLP~output'
range='-l -1 -u 1'

cd ../ntuples
for file in $(ls *mva_Sweighted.root)
do
  MCfile=$(echo $file | sed 's/data/MC/' | sed 's/_Sweighted//')
  plotname="../latex/figs/mvaOutput$(echo $file | sed -r 's/.*data(.*)_mva.*/\1/')_${branch}"
  ../bin/CompareBranch \
       -R $file \
       -M $MCfile \
       -W \"Nsig_sw\" \
       -B \"${branch}\" \
       -T \"${title}\" \
       -U \"${unit}\" \
       -O \"$plotname\" \
       ${range} \
       -b 100
done
exit 0



