#! /bin/bash
for mode in $(ls *_cuts.root | sed 's/_cuts\.root//'); do
root -q -b -l "addBranches.C+(\"${mode}\")"
done
exit 0
