#!/bin/bash
Bdmass="5279.58"
Kstmass="891.66"
Bdwindow="60"
Kstwindow="150"
#Simple cut on events that are both m(KKKpi) in Bd range and m(Kpi) in K* range
vetoes[0]="(TMath::Abs(KpiM-${Kstmass})>${Kstwindow}||TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow})"
##Tighten DLL if m(KKKpi) in Bd mass range
##Try 0.2 with this one
vetoes[1]="(TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow}||(TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.2&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.2))"
##Tighten DLL if m(Kpi) in K* mass range
#vetoes[2]="(TMath::Abs(KpiM-${Kstmass})>${Kstwindow}||(TMath::Abs(KpiM-${Kstmass})<${Kstwindow}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.1))"
##Tighten DLL if both m(KKKpi) in Bd range and m(Kpi) in K* range
#vetoes[3]="((TMath::Abs(KpiM-${Kstmass})>${Kstwindow}||TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow})||((TMath::Abs(KpiM-${Kstmass})<${Kstwindow}&&TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow})&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.1))"
##Require ProbNNk>ProbNNpi if m(KKKpi) in Bd mass range
vetoes[4]="(TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow}||(TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow}&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi))"
##Require ProbNNk>ProbNNpi if m(Kpi) in K* mass range
#vetoes[5]="(TMath::Abs(KpiM-${Kstmass})>${Kstwindow}||(TMath::Abs(KpiM-${Kstmass})<${Kstwindow}&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi))"
##Require ProbNNk>ProbNNpi if both m(KKKpi) in Bd range and m(Kpi) in K* range
#vetoes[6]="((TMath::Abs(KpiM-${Kstmass})>${Kstwindow}||TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow})||((TMath::Abs(KpiM-${Kstmass})<${Kstwindow}&&TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow})&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi))"
##Require pion ProbNNk>ProbNNpi if m(KKKpi) in Bd mass range
#vetoes[7]="(TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow}||(TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow}&&pion_ProbNNk>pion_ProbNNpi))"
##Require pion ProbNNk>ProbNNpi if m(Kpi) in K* mass range
#vetoes[8]="(TMath::Abs(KpiM-${Kstmass})>${Kstwindow}||(TMath::Abs(KpiM-${Kstmass})<${Kstwindow}&&pion_ProbNNk>pion_ProbNNpi))"
##Require pion ProbNNk>ProbNNpi if both m(KKKpi) in Bd range and m(Kpi) in K* range
#vetoes[9]="((TMath::Abs(KpiM-${Kstmass})>${Kstwindow}||TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow})||((TMath::Abs(KpiM-${Kstmass})<${Kstwindow}&&TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow})&&pion_ProbNNk>pion_ProbNNpi))"
# Tighten probNNk in Bd mass range AND require ProbNNk>ProbNNpi
vetoes[10]="(TMath::Abs(phiKpiM-${Bdmass})>${Bdwindow}||(TMath::Abs(phiKpiM-${Bdmass})<${Bdwindow}&&Kplus0_ProbNNk>0.2&&Kminus0_ProbNNk>0.2&&Kplus0_ProbNNpi<0.6&&Kminus0_ProbNNpi<0.6))"
################################################################################
i=0
for veto in ${vetoes[@]}; do
root -l -q -b "cuteff.C+(\"BsphiKK_MC_mvaVars\",\"B_s0_LOKI_Mass\",\"\",\"${veto}\")"
root -l -q -b "cuteff.C+(\"Bsphiphi_MC_mvaVars\",\"B_s0_LOKI_Mass\",\"\",\"${veto}\")"
root -l -q -b "cuteff.C+(\"BdphiKst_MC_mvaVars\",\"B_s0_LOKI_Mass\",\"\",\"${veto}\")"
echo "                ———"
root -l -q -b "cuteff.C+(\"BsphiKK_data_mvaVars\",\"B_s0_LOKI_Mass\",\"B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5550\",\"${veto}\",\"testphisktveto_data${i}\")"
echo "################################################################################"
i=$(($i+1))
done

## Check for helicity angle bias
#root -l -q -b "cuteff.C+(\"BsphiKK_data_mvaVars\",\"Phi_angle\",\"B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5550\",\"${vetoes[4]}\",\"testphisktveto_data4_Phi_angle\")"
#root -l -q -b "cuteff.C+(\"BsphiKK_data_mvaVars\",\"cos_theta1\",\"B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5550\",\"${vetoes[4]}\",\"testphisktveto_data4_costheta1\")"
#root -l -q -b "cuteff.C+(\"BsphiKK_data_mvaVars\",\"cos_theta2\",\"B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5550\",\"${vetoes[4]}\",\"testphisktveto_data4_costheta2\")"

exit 0
