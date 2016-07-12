#!/bin/bash
cd ..
bin/AngularAcceptance_AdaptiveHist ntuples/BsphiKK_MC_mvacut.root ntuples/BsphiKK_Gen_mvaVars.root
bin/AddBranches_Acceptance Acceptance.root ntuples/Bsphiphi_Gen_mvaVars.root Bsphiphi_Gen_mvaVars_acc_weights.root
exit 0

