#! /bin/bash
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r3p1/InstallArea/scripts/LbLogin.sh
###########################################################
cutapplier BsphiKK_data_bdtVars.root DecayTree "B_s0_LOKI_Mass>5600" BsphiKK_sideband_bdtVars.root
exit 0
