#!/bin/bash
## Cut-based selection
./docuts.sh 2>&1| tee log/docuts.log
./doPIDcalibcuts.sh 2>&1| tee log/doPIDcalibcuts.log
./addBranches.sh 2>&1| tee log/addBranches.log
./addBranches_Gen.sh 2>&1| tee log/addBranches_Gen.log
./dovetoes.sh 2>&1| tee log/dovetoes.log
./sweight.sh 2>&1| tee log/sweight.log
./MVA.sh
./killclones.sh 2>&1| tee log/killclones.log
source eos.sh
xrdcp --verbose --force ../ntuples/*mva.root ../ntuples/*mvacut*.root ../ntuples/*Gen*mvaVars* ${nTuples_dir}/ntuples/

