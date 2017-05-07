#!/bin/bash
cd ..
selfile='ntuples/BsphiKK_MC_mvacut'
genfile='ntuples/BsphiKK_Gen_mvaVars'
if [ ! -e ${selfile}_foracceptance.root ]
then
  cutapplier ${selfile}.root DecayTree "BCON_KK_M<1800&&abs(KK_TRUEID)>500" ${selfile}_foracceptance.root
fi
if [ ! -e ${genfile}_foracceptance.root ]
then
  cutapplier ${genfile}.root MCDecayTree "KK_M<1800" ${genfile}_foracceptance.root
fi
bin/AngularAcceptance_AdaptiveHist ${selfile}_foracceptance.root ${genfile}_foracceptance.root 100
#bin/AddBranches_Acceptance Acceptance.root ntuples/Bsphiphi_Gen_mvaVars.root ntuples/Bsphiphi_Gen_mvaVars_acc_weights.root
#bin/AddBranches_Acceptance Acceptance.root ntuples/Bsphiphi_MC_mvacut.root ntuples/Bsphiphi_MC_mvacut_acc_weights.root
#bin/AddBranches_Acceptance Acceptance.root ntuples/BsphiKK_MC_mvacut.root ntuples/BsphiKK_MC_mvacut_acc_weights.root
#bin/AddBranches_Acceptance Acceptance.root ntuples/BsphiKK_data_mvacut.root ntuples/BsphiKK_data_mvacut_acc_weights.root
source eos.sh
cp -v Acceptance.root ${nTuples_dir}
return 0

