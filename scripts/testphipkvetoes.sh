#! /bin/bash
Lbwidth="50"
#Simple cut on events that have m(KKKp) in Lb range
vetoes[0]="(TMath::Abs(phiKpM-5620)>${Lbwidth})"
#Tighten DLL if m(KKKp) in Lb mass range
vetoes[1]="(TMath::Abs(phiKpM-5620)>${Lbwidth}||(TMath::Abs(phiKpM-5620)<${Lbwidth}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.1))"
#Tighter!
vetoes[2]="(TMath::Abs(phiKpM-5620)>${Lbwidth}||(TMath::Abs(phiKpM-5620)<${Lbwidth}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.2&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.2))"
#Require ProbNNk>ProbNNp if m(KKKp) in Lb mass range
vetoes[3]="(TMath::Abs(phiKpM-5620)>${Lbwidth}||(TMath::Abs(phiKpM-5620)<${Lbwidth}&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp))"
################################################################################
for veto in ${vetoes[@]}; do
root -l -q -b "cuteff.C+(\"BsphiKK_MC_bdtVars\",\"B_s0_LOKI_Mass\",\"\",\"${veto}\")"
root -l -q -b "cuteff.C+(\"LbphiKp_MC_bdtVars\",\"B_s0_LOKI_Mass\",\"\",\"${veto}\")"
root -l -q -b "cuteff.C+(\"BsphiKK_data_bdtVars\",\"B_s0_LOKI_Mass\",\"TMath::Abs(B_s0_LOKI_Mass-5366.77)<${Lbwidth}\",\"${veto}\")"
echo "################################################################################"
done
exit 0
