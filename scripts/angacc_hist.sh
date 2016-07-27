#!/bin/bash
cd ..
bin/AngularAcceptance_AdaptiveHist ntuples/BsphiKK_MC_mvacut.root ntuples/BsphiKK_Gen_5M_1800_mvaVars.root
bin/AddBranches_Acceptance Acceptance.root ntuples/Bsphiphi_Gen_mvaVars.root ntuples/Bsphiphi_Gen_mvaVars_acc_weights.root
bin/AddBranches_Acceptance Acceptance.root ntuples/BsphiKK_MC_mvacut.root ntuples/BsphiKK_MC_mvacut_acc_weights.root
bin/AddBranches_Acceptance Acceptance.root ntuples/BsphiKK_data_mvacut.root ntuples/BsphiKK_data_mvacut_acc_weights.root
exit 0

