#!/bin/bash
cd ../ntuples/
../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_mvacut.root \
    -R ../ntuples/BsphiKK_data_mvacut.root \
    -O ../latex/figs/Bsmassfit_mvacut \
    --pulls

../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1050_mvacut.root \
    -R ../ntuples/BsphiKK_data_1050_mvacut.root \
    -O ../latex/figs/Bsmassfit_1050_mvacut \
    --pulls

../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1800_mvacut.root \
    -R ../ntuples/BsphiKK_data_1800_mvacut.root \
    -O ../latex/figs/Bsmassfit_1800_mvacut \
    --pulls

../bin/BsMassFit \
    -M ../ntuples/BsphiKK_MC_1050_1800_mvacut.root \
    -R ../ntuples/BsphiKK_data_1050_1800_mvacut.root \
    -O ../latex/figs/Bsmassfit_1050_1800_mvacut \
    --pulls

