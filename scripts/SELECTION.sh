#!/bin/bash
## Cut-based selection
./docuts.sh | tee log/docuts.log
./addBranches.sh | tee log/addBranches.log
./addBranches_Gen.sh | tee log/addBranches_Gen.log
./dovetoes.sh | tee log/dovetoes.log
./sweight.sh | tee log/sweight.log
./MVA.sh
./killclones.sh | tee log/killclones.log
source eos.sh
cp -v ../ntuples/*mvacut*.root ../ntuples/*Gen*mvaVars* ${nTuples_dir}/ntuples/
exit 0

