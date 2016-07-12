#!/bin/bash
cd ../ntuples/
../bin/BsMassFit -M ../ntuples/Bsphiphi_MC_mva.root \
                 -R ../ntuples/BsphiKK_data_mva.root \
                 -C "TMath::Abs(KK_M-1019.461)<15" \
                 -O ../latex/figs/Bsphiphi_fit_nomva \
                 --pulls
../bin/BsMassFit -M ../ntuples/Bsphiphi_MC_mva.root \
                 -R ../ntuples/BsphiKK_data_mva.root \
                 -C "TMath::Abs(KK_M-1019.461)<15&&mlp>${optcut}" \
                 -O ../latex/figs/Bsphiphi_fit_mvacut \
                 --pulls
../bin/GetCutEff ../ntuples/Bsphiphi_MC_mva.root "mlp>${optcut}" --save --output-file ../scripts/tables/mvaeffs.csv --result-name BsphiphiMCmvacuteff
../bin/GetCutEff ../ntuples/BsphiKK_MC_1800_mva.root  "mlp>${optcut}" --save --output-file ../scripts/tables/mvaeffs.csv --result-name BsphiKKMCmvacuteff
../bin/GetCutEff ../ntuples/BsphiKK_sideband_1050_1800_mva.root  "mlp<${optcut}" --save --output-file ../scripts/tables/mvaeffs.csv --result-name sidebandmvacutrej
../bin/ExportResults ../scripts/tables/mvaeffs.csv ../latex/results/mvaeffs.tex
exit 0

