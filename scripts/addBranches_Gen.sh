#! /bin/bash
cd ../ntuples/
for mode in $(ls *_Gen.root | sed 's/\.root//'); do
../bin/AddBranches_Gen ${mode}
done
return 0

