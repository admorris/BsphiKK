#!/bin/bash
## Cut-based selection
./docuts.sh | tee log/docuts.log
./addBranches.sh | tee log/addBranches.log
./dovetoes.sh | tee log/dovetoes.log
./sweight.sh | tee log/sweight.log
## Multivariate selection
./trainmva.sh | tee log/trainmva.log
./applymva.sh | tee log/applymva.log
./mvacut.sh | tee log/mvacut.log
