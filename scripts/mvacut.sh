#! /bin/bash
for mode in $(ls ../ntuples/*_mva.root | sed 's/_mva\.root//'); do
cutapplier ${mode}_mva.root DecayTree "B_s0_LOKI_Mass>5250&&B_s0_LOKI_Mass<5500&&mlp>0.88" ${mode}_mvacut.root
done
exit 0
