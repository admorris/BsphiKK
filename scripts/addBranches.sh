#! /bin/bash
cd ../ntuples/
for mode in $(ls *_cuts.root | sed 's/_cuts\.root//'); do
../bin/AddBranches ${mode}_cuts.root ${mode}_mvaVars.root &
done
wait
exit 0
