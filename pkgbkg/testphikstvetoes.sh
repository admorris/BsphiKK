#! /bin/bash
Bdwidth="50"
Kstwidth="150"
#Tighten DLL if m(KKKpi) in Bd mass range
vetoes[0]="(TMath::Abs(phiKpiM-5279.58)>${Bdwidth}||(TMath::Abs(phiKpiM-5279.58)<${Bdwidth}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.1))"
#Tighten DLL if m(Kpi) in K* mass range
vetoes[1]="(TMath::Abs(KpiM-891.66)>${Kstwidth}||(TMath::Abs(KpiM-891.66)<${Kstwidth}&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.1&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.1))"
#Require ProbNNk>ProbNNpi if m(KKKpi) in Bd mass range
vetoes[2]="(TMath::Abs(phiKpiM-5279.58)>${Bdwidth}||(TMath::Abs(phiKpiM-5279.58)<${Bdwidth}&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi))"
#Require ProbNNk>ProbNNpi if m(Kpi) in K* mass range
vetoes[3]="(TMath::Abs(KpiM-891.66)>${Kstwidth}||(TMath::Abs(KpiM-891.66)<${Kstwidth}&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi))"
#Simple cut on events that are both m(KKKpi) in Bd range and m(Kpi) in K* range
vetoes[4]="(TMath::Abs(KpiM-891.66)>${Kstwidth}||TMath::Abs(phiKpiM-5279.58)>${Bdwidth})"
################################################################################
for veto in ${vetoes[@]}; do
root -l -q -b "cuteff.C+(\"BsphiKK_MC_bdtVars\",\"B_s0_LOKI_Mass\",\"\",\"${veto}\")"
root -l -q -b "cuteff.C+(\"BdphiKst_MC_bdtVars\",\"B_s0_LOKI_Mass\",\"\",\"${veto}\")"
root -l -q -b "cuteff.C+(\"BsphiKK_data_bdtVars\",\"B_s0_LOKI_Mass\",\"TMath::Abs(B_s0_LOKI_Mass-5366.77)<${Bdwidth}\",\"${veto}\")"
echo "################################################################################"
done
exit 0
