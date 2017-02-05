#!/bin/bash
strippingphiwindow="-l 994.461 -u 1044.461"
phiwindow="-l 1004.461 -u 1034.461"
lowmKKwindow="-l 980 -u 1080"
lowmKKwindowGeV="-l 0.980 -u 1.080"
medmKKwindow="-l 980 -u 1200"
medmKKwindowGeV="-l 0.980 -u 1.200"
allmKKwindow="-l 980 -u 1800"
allmKKwindowGeV="-l 0.980 -u 1.800"
cd ..
bin/mKKfit -F ntuples/Bsphiphi_MC_mvacut.root ${lowmKKwindowGeV} -b 100 -B KK_TRUEM_GeV -O latex/figs/phiphi_MC_KK_fit_1080_GeV_acceptance -U "GeV/#it{c}^{2}" --fitacc
bin/mKKfit -F ntuples/BsphiKK_MC_mvacut.root ${allmKKwindowGeV} -b 100 -C "abs(KK_TRUEID)>500" -B KK_TRUEM_GeV -O latex/figs/phiKK_MC_KK_fit_1080_GeV_acceptance -U "GeV/#it{c}^{2}" --fitacc --fitnonres --dontfitphi
exit 0

