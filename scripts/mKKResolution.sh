#!/bin/bash
cd ../ntuples
../bin/GetResolution -F ../ntuples/Bsphiphi_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O ../latex/figs/mKKresolution
exit 0

