#!/bin/bash
## Multivariate selection
cd ../mva
rm *.d *.so *.pcm
cd -
./trainmva.sh | tee log/trainmva.log
./applymva.sh | tee log/applymva.log
./mvagetoptcut.sh | tee log/mvagetoptcut.log
./mvacut.sh | tee log/mvacut.log


