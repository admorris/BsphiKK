#!/bin/bash
cd ../ntuples
../bin/GetResolution -F ../ntuples/BsphiKK_MC_1800_mvacut.root -P Kplus0 Kminus0 -B KK_M -l -30 -u 30 -O ../latex/figs/mKKresolution
