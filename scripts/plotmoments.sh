#!/bin/bash
cd ../ntuples
../bin/PlotMoments --file BsphiKK_data_mvacut_final_Sweighted.root --weight Nsig_sw --mass BCON_KK_M --costheta BCON_cos_theta2 --cuts "BCON_KK_M<1800" --plot ../latex/figs/moments_low_mKK -l 980 -u 1080 -b 100
../bin/PlotMoments --file BsphiKK_data_mvacut_final_Sweighted.root --weight Nsig_sw --mass BCON_KK_M --costheta BCON_cos_theta2 --cuts "BCON_KK_M<1800" --plot ../latex/figs/moments_high_mKK -l 1080 -u 1800 -b 36
