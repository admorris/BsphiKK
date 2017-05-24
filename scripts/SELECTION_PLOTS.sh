#!/bin/bash
./showvetoes.sh 2>&1| tee log/showvetoes.log
## Preliminary m(KK) plots
./plotmKK.sh 2>&1| tee log/plotmKK.log
## MVA data-MC comparison
./mvadataMCsWcomp.sh 2>&1| tee log/mvadataMCsWcomp.log
./mvadataMCsWcompOutput.sh 2>&1| tee log/mvadataMCsWcompOutput.log
## General MVA plots about the training
./mvacuteff.sh 2>&1| tee log/mvacuteff.log
./mvaplots.sh 2>&1| tee log/mvaplots.log
## Plot fully-selected m(KK) and helicity angles
./plotmKK_mva.sh 2>&1| tee log/plotmKK_mva.log
./plot4D.sh 2>&1| tee log/plot4D.log


