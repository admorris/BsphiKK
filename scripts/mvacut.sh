#! /bin/bash
source mvaopticut.sh
for mode in $(ls ../ntuples/*_mva.root | sed 's/_mva\.root//'); do
cutapplier ${mode}_mva.root DecayTree "mlp>${optcut}" ${mode}_mvacut.root &
done
wait


