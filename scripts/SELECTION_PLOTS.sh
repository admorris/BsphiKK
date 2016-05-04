#!/bin/bash
./showvetoes.sh | tee log/showvetoes.log
## Efficiencies
./vetoeff.sh | tee log/vetoeff.log
./seleff.sh
./trigeff.sh
## Preliminary m(KK) plots
./plotmKK.sh
## MVA stuff
./mvadataMCsWcomp.sh | tee log/mvadataMCsWcomp.log
./mvaplots.sh | tee log/mvaplots.log
./mvadataMCsWcompOutput.sh | tee log/mvadataMCsWcompOutput.log
./mvacuteff.sh | tee log/mvacuteff.log
## Plot fully-selected m(KK) and helicity angles
./plotmKK_mva.sh | tee log/plotmKK_mva.log
./plot4D.sh | tee log/plot4D.log
