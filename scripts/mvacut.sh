#! /bin/bash
for mode in $(ls ../ntuples/*_mva.root | sed 's/_mva\.root//'); do
cutapplier ${mode}_mva.root DecayTree "mlp>0.7159" ${mode}_mvacut.root
done
exit 0
