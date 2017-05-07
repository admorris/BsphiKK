#!/bin/bash
cd ..
bin/CompareMoments --MCfile toys_alt.root --MCmass mKK --MCangle ctheta_2 --lower 1.05 --bins 25 --CDfile ntuples/BsphiKK_data_mvacut_1800_Sweighted.root --CDweight Nsig_sw --CDcut "B_s0_LOKI_Mass>5326.12&&5406.56>B_s0_LOKI_Mass"
return 0

