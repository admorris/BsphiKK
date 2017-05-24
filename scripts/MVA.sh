#!/bin/bash
## Multivariate selection
cd ../mva
rm *.d *.so *.pcm
cd -
./trainmva.sh 2>&1| tee log/trainmva.log
./applymva.sh 2>&1| tee log/applymva.log
./mvagetoptcut.sh 2>&1| tee log/mvagetoptcut.log
./mvacut.sh 2>&1| tee log/mvacut.log


