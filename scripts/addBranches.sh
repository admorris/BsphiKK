#! /bin/bash
cd ../ntuples/
for mode in $(ls *_cuts.root | sed 's/_cuts\.root//'); do
../bin/AddBranches ${mode}
done
exit 0
