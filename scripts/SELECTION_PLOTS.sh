#!/bin/bash
./showvetoes.sh | tee log/showvetoes.log
## Preliminary m(KK) plots
./plotmKK.sh | tee log/plotmKK.log
## MVA data-MC comparison
./mvadataMCsWcomp.sh | tee log/mvadataMCsWcomp.log
./mvadataMCsWcompOutput.sh | tee log/mvadataMCsWcompOutput.log
## General MVA plots about the training
./mvacuteff.sh | tee log/mvacuteff.log
./mvaplots.sh | tee log/mvaplots.log
## Plot fully-selected m(KK) and helicity angles
./plotmKK_mva.sh | tee log/plotmKK_mva.log
./plot4D.sh | tee log/plot4D.log
exit 0

