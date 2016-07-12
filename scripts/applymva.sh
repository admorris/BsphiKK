#! /bin/bash
cd ../mva
for filename in $(ls ../ntuples/*mvaVars_vetoes*root)
do
  root -l -q -b "ZTMVAClassificationApplication.C+(\"$filename\")"
done
exit 0

