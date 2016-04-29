#!/bin/bash
cd ../ntuples/
../bin/BsMassFit \
    -M BsphiKK_MC_mvacut.root \
    -R BsphiKK_data_mvacut.root \
    -O ../latex/figs/Bsmassfit_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls

../bin/BsMassFit \
    -M BsphiKK_MC_1050_mvacut.root \
    -R BsphiKK_data_1050_mvacut.root \
    -O ../latex/figs/Bsmassfit_1050_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls

../bin/BsMassFit \
    -M BsphiKK_MC_1800_mvacut.root \
    -R BsphiKK_data_1800_mvacut.root \
    -O ../latex/figs/Bsmassfit_1800_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls \
    --draw-region 2 \
    --backgrounds ../f1420_Bs0_branches.root/HISTPDF BdphiKst_MC_mvacut.root LbphiKp_MC_mvacut.root \
    --yields 0 0.02 475 \
    --yopts flo flo abs \
    --logy

../bin/BsMassFit \
    -M BsphiKK_MC_1050_1800_mvacut.root \
    -R BsphiKK_data_1050_1800_mvacut.root \
    -O ../latex/figs/Bsmassfit_1050_1800_mvacut \
    -N B_s0_LOKI_Mass \
    --pulls

