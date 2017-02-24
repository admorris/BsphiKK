#!/bin/bash
cd ../
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_1 -C "BCON_KK_M>1000&&BCON_KK_M<1100&&KK_TRUEID!=333" | tee KKresolution1.log
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_2 -C "BCON_KK_M>1100&&BCON_KK_M<1200&&KK_TRUEID!=333" | tee KKresolution2.log
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_3 -C "BCON_KK_M>1200&&BCON_KK_M<1300&&KK_TRUEID!=333" | tee KKresolution3.log
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_4 -C "BCON_KK_M>1300&&BCON_KK_M<1400&&KK_TRUEID!=333" | tee KKresolution4.log
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_5 -C "BCON_KK_M>1400&&BCON_KK_M<1500&&KK_TRUEID!=333" | tee KKresolution5.log
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_6 -C "BCON_KK_M>1500&&BCON_KK_M<1600&&KK_TRUEID!=333" | tee KKresolution6.log
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_7 -C "BCON_KK_M>1600&&BCON_KK_M<1700&&KK_TRUEID!=333" | tee KKresolution7.log
bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_8 -C "BCON_KK_M>1700&&BCON_KK_M<1800&&KK_TRUEID!=333" | tee KKresolution8.log
exit 0

