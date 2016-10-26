#!/bin/bash
## Cut-based selection
./docuts.sh | tee log/docuts.log
./addBranches.sh | tee log/addBranches.log
./dovetoes.sh | tee log/dovetoes.log
./sweight.sh | tee log/sweight.log
./killclones.sh | tee log/killclones.log
./MVA.sh
exit 0

