#!/bin/bash
Bdmass="5279.58"
Kstmass="891.66"
Bdwindow="60" # Try 60
Kstwindow="150"
Lbmass="5619.5"
Lbwindow="60" # Try 60
Lcmass="2286.46"
Lcwindow="24"
Dsmass="1968.3"
Dswindow="24"
Dumass="1864.84"
Duwindow="24"
Ddmass="1869.61"
Ddwindow="24"
phimass="1019.461"
phiwindow="15"
###############################################################################
#Removed Hlt2Topo2BodyBBDTDecision_TOS and Hlt2IncPhiDecision_TOS because they bias the mass distribution
trigcut="(B_s0_L0HadronDecision_TOS||B_s0_L0Global_TIS)&&B_s0_Hlt1TrackAllL0Decision_TOS&&(B_s0_Hlt2Topo3BodyBBDTDecision_TOS||B_s0_Hlt2Topo4BodyBBDTDecision_TOS)"
#Tighter cut on track ghost prob than stripping
ghstcut="Kminus_TRACK_GhostProb<0.3&&Kplus_TRACK_GhostProb<0.3&&Kminus0_TRACK_GhostProb<0.3&&Kplus0_TRACK_GhostProb<0.3"
#Get rid of K->mu
trackisMuoncut="Kminus_isMuon==0&&Kplus_isMuon==0&&Kminus0_isMuon==0&&Kplus0_isMuon==0"
#Kaon PT>500 (as in PhiRhoLine) to all Kaons
KpTcut="Kminus_PT>500&&Kplus_PT>500&&Kminus0_PT>500&&Kplus0_PT>500"
#Phi mass window cut
phiMcut="TMath::Abs(phi_1020_LOKI_Mass-${phimass})<${phiwindow}"
## Symmetrise the stripping cuts
#phi and rho IP chi2 cuts
phiIPCHI2cut="phi_1020_IPCHI2_OWNPV>16"
KKIPCHI2cut="KK_IPCHI2_OWNPV>16"
# KK vertex chi2 cut
KKEVCHI2cut="KK_ENDVERTEX_CHI2<25"
# stdtightkaons
StdTightKaons="Kminus0_PIDK>0&&Kplus0_PIDK>0"
#Bs mass window cut
BsMcut="B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5600"
sidebandcut="B_s0_LOKI_Mass>5500"
#Bs flight distance chi-squared cut
BsFDCHI2cut="B_s0_FDCHI2_OWNPV>250"
BsIPCHI2cut="B_s0_IPCHI2_OWNPV<20"
#PID cuts to get better Kaons
KpiPIDval="0.025"
KpPIDval="0.01"
KpiPIDcut="Kminus_ProbNNk*(1-Kminus_ProbNNpi)>${KpiPIDval}&&Kplus_ProbNNk*(1-Kplus_ProbNNpi)>${KpiPIDval}&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>${KpiPIDval}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>${KpiPIDval}"
KpPIDcut="Kplus_ProbNNk*(1-Kplus_ProbNNp)>${KpPIDval}&&Kminus_ProbNNk*(1-Kminus_ProbNNp)>${KpPIDval}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>${KpPIDval}&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>${KpPIDval}"
#Monte Carlo background category
BKGCATcut="(B_s0_BKGCAT<20||B_s0_BKGCAT==50)"
###############################################################################
phikstveto="(TMath::Abs(phiKpluspiminusM-${Bdmass})>${Bdwindow}&&TMath::Abs(KpluspiminusM-${Kstmass})>${Kstwindow}&&TMath::Abs(phiKminuspiplusM-${Bdmass})>${Bdwindow}&&TMath::Abs(KminuspiplusM-${Kstmass})>${Kstwindow})||(((TMath::Abs(phiKpluspiminusM-${Bdmass})<${Bdwindow}||TMath::Abs(KpluspiminusM-${Kstmass})<${Kstwindow})&&Kminus0_ProbNNk>Kminus0_ProbNNpi)&&((TMath::Abs(phiKminuspiplusM-${Bdmass})<${Bdwindow}||TMath::Abs(KminuspiplusM-${Kstmass})<${Kstwindow})&&Kplus0_ProbNNk>Kplus0_ProbNNpi))"
LbphiKpveto="(TMath::Abs(phiKpluspbarM-${Lbmass})>${Lbwindow}&&TMath::Abs(phiKminuspM-${Lbmass})>${Lbwindow})||(((TMath::Abs(phiKpluspbarM-${Lbmass})<${Lbwindow})&&Kminus0_ProbNNk>Kminus0_ProbNNp)&&((TMath::Abs(phiKminuspM-${Lbmass})<${Lbwindow})&&Kplus0_ProbNNk>Kplus0_ProbNNp))"
Lcphipveto="(TMath::Abs(phipM-${Lcmass})>${Lcwindow}||(TMath::Abs(phipM-${Lcmass})<${Lcwindow}&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp))&&(TMath::Abs(phipbarM-${Lcmass})>${Lcwindow}||(TMath::Abs(phipbarM-${Lcmass})<${Lcwindow}&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp))"
DtoKaonsveto="TMath::Abs(phiKplusM-${Dsmass})>${Dswindow}&&TMath::Abs(phiKminusM-${Dsmass})>${Dswindow}"
Dtophipiveto="TMath::Abs(phipiplusM-${Ddmass})>${Ddwindow}&&TMath::Abs(phipiminusM-${Ddmass})>${Ddwindow}&&TMath::Abs(phipiplusM-${Dsmass})>${Dswindow}&&TMath::Abs(phipiminusM-${Dsmass})>${Dswindow}"
vetoes=($phikstveto $LbphiKpveto $Lcphipveto $DtoKaonsveto $Dtophipiveto)

