#! /bin/bash
Bdmass="5279.58"
Kstmass="891.66"
Bdwindow="50"
Kstwindow="150"
Lbmass="5619.5"
Lbwindow="30"
Lcmass="2286.46"
Lcwindow="24"
Dsmass="1968.3"
Dswindow="24"
Dumass="1864.84"
Duwindow="24"
phikstveto="(TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow}||(TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow}&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi))"
LbphiKpveto="(TMath::Abs(phiKpM-${Lbmass})>${Lbwindow}||(TMath::Abs(phiKpM-${Lbmass})<${Lbwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.1))"
Lcphipveto="(TMath::Abs(phipM-${Lcmass})>${Lcwindow}||(TMath::Abs(phipM-${Lcmass})<${Lcwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.1))&&(TMath::Abs(phipbarM-${Lcmass})>${Lcwindow}||(TMath::Abs(phipbarM-${Lcmass})<${Lcwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.1))"
DtoKaonsveto="TMath::Abs(phiKplusM-${Dsmass})>${Dswindow}&&TMath::Abs(phiKminusM-${Dsmass})>${Dswindow}&&TMath::Abs(KK_MM-${Dumass})>${Duwindow}"
Dtophipiveto="TMath::Abs(phipiplusM-${Dsmass})>${Dswindow}&&TMath::Abs(phipiminusM-${Dsmass})>${Dswindow}"
################################################################################
for mode in $(ls *_bdtVars.root | sed 's/_bdtVars\.root//'); do
cutapplier ${mode}_bdtVars.root DecayTree "${phikstveto}&&${LbphiKpveto}&&${Lcphipveto}&&${DtoKaonsveto}&&${Dtophipiveto}" ${mode}_bdtVars_vetoes.root
done
exit 0
