#!/bin/bash
Bdmass="5279.58"
Kstmass="891.66"
Bdwindow="50" # Try 60
Kstwindow="150"
Lbmass="5619.5"
Lbwindow="30" # Try 60
Lcmass="2286.46"
Lcwindow="24"
Dsmass="1968.3"
Dswindow="24"
Dumass="1864.84"
Duwindow="24"
phimass="1019.461"
phiwindow="15"
###############################################################################
#Same trigger lines as Haofei's analysis
trigcut="(B_s0_L0HadronDecision_TOS||B_s0_L0Global_TIS)&&B_s0_Hlt1TrackAllL0Decision_TOS&&(B_s0_Hlt2Topo2BodyBBDTDecision_TOS||B_s0_Hlt2Topo3BodyBBDTDecision_TOS||B_s0_Hlt2Topo4BodyBBDTDecision_TOS||B_s0_Hlt2IncPhiDecision_TOS)"
#Tighter cut on track ghost prob than stripping
ghstcut="Kminus_TRACK_GhostProb<0.3&&Kplus_TRACK_GhostProb<0.3&&Kminus0_TRACK_GhostProb<0.3&&Kplus0_TRACK_GhostProb<0.3"
#Get rid of K->mu
trackisMuoncut="Kminus_isMuon==0&&Kplus_isMuon==0&&Kminus0_isMuon==0&&Kplus0_isMuon==0"
#Kaon PT>500 (as in PhiRhoLine) to all Kaons
KpTcut="Kminus_PT>500&&Kplus_PT>500&&Kminus0_PT>500&&Kplus0_PT>500"
#Phi mass window cut
phiMcut="TMath::Abs(phi_1020_MM-${phimass})<${phiwindow}"
#Bs mass window cut
BsMcut="B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5600"
#Bs flight distance chi-squared cut
BsFDCHI2cut="B_s0_FDCHI2_OWNPV>250"
BsIPCHI2cut="B_s0_IPCHI2_OWNPV<20"
#PID cuts to get better Kaons
#KPIDcut="Kminus_ProbNNk>0.1&&Kplus_ProbNNk>0.1&&Kminus0_ProbNNk>0.1&&Kplus0_ProbNNk>0.1"
KpiPIDcut="Kminus_ProbNNk*(1-Kminus_ProbNNpi)>0.025&&Kplus_ProbNNk*(1-Kplus_ProbNNpi)>0.025&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.025&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.025"
KpPIDcut="Kplus_ProbNNk*(1-Kplus_ProbNNp)>0.01&&Kminus_ProbNNk*(1-Kminus_ProbNNp)>0.01&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.01&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.01"
#Monte Carlo background category
BKGCATcut="(B_s0_BKGCAT<20||B_s0_BKGCAT==50)"
###############################################################################
phikstveto="(TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow}||(TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow}&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi))"
# Try tightening this
LbphiKpveto="(TMath::Abs(phiKpM-${Lbmass})>${Lbwindow}||(TMath::Abs(phiKpM-${Lbmass})<${Lbwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.1))"
Lcphipveto="(TMath::Abs(phipM-${Lcmass})>${Lcwindow}||(TMath::Abs(phipM-${Lcmass})<${Lcwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.1))&&(TMath::Abs(phipbarM-${Lcmass})>${Lcwindow}||(TMath::Abs(phipbarM-${Lcmass})<${Lcwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.1))"
DtoKaonsveto="TMath::Abs(phiKplusM-${Dsmass})>${Dswindow}&&TMath::Abs(phiKminusM-${Dsmass})>${Dswindow}"
Dtophipiveto="TMath::Abs(phipiplusM-${Dsmass})>${Dswindow}&&TMath::Abs(phipiminusM-${Dsmass})>${Dswindow}"
