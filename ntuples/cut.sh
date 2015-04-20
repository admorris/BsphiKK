#! /bin/bash

#hadd -f BsphiKK_data_nocut.root BsphiKK_421.root BsphiKK_422.root BsphiKK_423.root BsphiKK_424.root
#hadd -f BsphiKK_MC_nocut.root BsphiKK_431.root BsphiKK_432.root
#hadd -f Bsphiphi_MC_nocut.root BsphiKK_429.root BsphiKK_430.root

#Same trigger lines as Haofei's analysis
trigcut="(B_s0_L0HadronDecision_TOS||B_s0_L0Global_TIS)&&B_s0_Hlt1TrackAllL0Decision_TOS&&(B_s0_Hlt2Topo2BodyBBDTDecision_TOS||B_s0_Hlt2Topo3BodyBBDTDecision_TOS||B_s0_Hlt2Topo4BodyBBDTDecision_TOS||B_s0_Hlt2IncPhiDecision_TOS)"
#Tighter cut on track ghost prob than stripping
ghstcut="Kminus_TRACK_GhostProb<0.3&&Kplus_TRACK_GhostProb<0.3&&Kminus0_TRACK_GhostProb<0.3&&Kplus0_TRACK_GhostProb<0.3"
#Kaon PT>500 (as in PhiRhoLine) to all Kaons
KpTcut="Kminus_PT>500&&Kplus_PT>500&&Kminus0_PT>500&&Kplus0_PT>500"
#Phi mass window cut
phiMcut="TMath::Abs(phi_1020_MM-1019.461)<15"
#Bs mass window cut
BsMcut="TMath::Abs(B_s0_MM-5366.77)<150"
#Bs flight distance chi-squared cut
BsFDCHI2cut="B_s0_FDCHI2_OWNPV>300"
#Same PID variable cut as Haofei's analysis
KPIDcut="Kminus_ProbNNk*(1-Kminus_ProbNNk)>0.025&&Kplus_ProbNNk*(1-Kplus_ProbNNk)>0.025&&Kminus0_ProbNNk*(1-Kminus0_ProbNNk)>0.025&&Kplus0_ProbNNk*(1-Kplus0_ProbNNk)>0.025"
#Alternative PID cut
KPIDcut="Kminus_ProbNNk>0.2&&Kplus_ProbNNk>0.2&&Kminus0_>0.2&&Kplus0_ProbNNk>0.2"
#Sum of all cuts
totalcut="${trigcut}&&${ghstcut}&&${phiMcut}&&${KpTcut}&&${BsMcut}&&${BsFDCHI2cut}&&${KPIDcut}"

cutapplier BsphiKK_data_nocut.root DecayTreeTuple/DecayTree "${totalcut}" BsphiKK_data_duplicates.root
cutapplier BsphiKK_MC_nocut.root DecayTreeTuple/DecayTree "${totalcut}" BsphiKK_MC_duplicates.root
cutapplier Bsphiphi_MC_nocut.root DecayTreeTuple/DecayTree "${totalcut}" Bsphiphi_MC_duplicates.root

root -q -b FlagClones.C+

cutapplier BsphiKK_data_duplicates_Clone.root DecayTree "isAlive==0" BsphiKK_data.root
cutapplier BsphiKK_MC_duplicates_Clone.root DecayTree "isAlive==0" BsphiKK_MC.root
cutapplier Bsphiphi_MC_duplicates_Clone.root DecayTree "isAlive==0" Bsphiphi_MC.root


