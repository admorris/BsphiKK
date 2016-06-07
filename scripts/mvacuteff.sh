#!/bin/bash
cd ../ntuples/
../bin/BsMassFit -M ../ntuples/Bsphiphi_MC_mva.root \
                 -R ../ntuples/BsphiKK_data_mva.root -C "TMath::Abs(KK_M-1019.461)<15" \
                 -O ../latex/figs/Bsphiphi_fit_nomva \
                 --pulls
../bin/BsMassFit -M ../ntuples/Bsphiphi_MC_mvacut.root \
                 -R ../ntuples/BsphiKK_data_mvacut.root \
                 -C "TMath::Abs(KK_M-1019.461)<15" \
                 -O ../latex/figs/Bsphiphi_fit_mvacut \
                 --pulls
