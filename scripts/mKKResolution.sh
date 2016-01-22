#!/bin/bash
cd ../ntuples
../bin/GetResolution -F ../ntuples/BsphiKK_MC_1800_mvacut.root -P phi_1020 -B phi_1020_M -l -5 -u 5 -O ../latex/figs/mKKresolution
