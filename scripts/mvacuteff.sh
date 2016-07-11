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
../bin/GetCutEff ../ntuples/Bsphiphi_MC_mva.root "mlp>0.62" --save --output-file ../scripts/tables/mvaeffs.csv --result-name BsphiphiMCmvacuteff
../bin/GetCutEff ../ntuples/BsphiKK_MC_mva.root  "mlp>0.62" --save --output-file ../scripts/tables/mvaeffs.csv --result-name BsphiKKMCmvacuteff
../bin/ExportResults ../scripts/tables/mvaeffs.csv ../latex/results/mvaeffs.tex
