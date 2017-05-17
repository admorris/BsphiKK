#!/bin/bash
source cuts.sh
cd ..
kaons=("Kminus0" "Kplus0")
#Lbwindow=50 # by default get these from cuts.sh
#Bdwindow=50 # uncomment if you want to test the effect of different window sizes
Lbmasses=("phiKpluspbarM" "phiKminuspM")
Bdmasses=("phiKpluspiminusM" "phiKminuspiplusM")
Lbdata="--Sfile ntuples/BsphiKK_MC_mvaVars.root --Bfile ntuples/LbphiKp_MC_mvaVars.root --cuts BCON_KK_M<1800"
Bddata="--Sfile ntuples/BsphiKK_MC_mvaVars.root --Bfile ntuples/BdphiKst_MC_mvaVars.root --cuts BCON_KK_M<1800"
vmin=0.00
vmax=1.00
nsteps=20
plotoptions="--varmin $vmin --varmax $vmax --xmin 0.4 --ymin 0.6 --samples $nsteps"
######################################################################################

inwindowcut=""
compinwindowcut=""
for particle in ${kaons[@]}
do
  inwindowcut+="&&(${particle}_ProbNNk*(1-${particle}_ProbNNp))>{}"
  compinwindowcut+="&&(${particle}_ProbNNk>${particle}_ProbNNp)"
done
bin/ROCcurve ${Lbdata} \
             --expr "abs(phiKpM-${Lbmass})>${Lbwindow}||(abs(phiKpM-${Lbmass})<${Lbwindow}${inwindowcut})" \
             --comp "abs(phiKpM-${Lbmass})>${Lbwindow}||(abs(phiKpM-${Lbmass})<${Lbwindow}${compinwindowcut})" \
             ${plotoptions} \
             --plot "latex/figs/prodPNNkp_ROC" \
             --blurb "#splitline{P_{NN}^{K}(1 #minus P_{NN}^{p}) < [$vmin, $vmax] on both tracks}{#scale[0.8]{in |m(#phi Kp) #minus m(#Lambda_{b})| < ${Lbwindow} MeV using best proton}}"

bin/ROCcurve ${Lbdata} \
             --expr "abs(phiKpM-${Lbmass})>${Lbwindow}||(abs(phiKpM-${Lbmass})<${Lbwindow}&&proton_ProbNNk*(1-proton_ProbNNp)>{})" \
             --comp "abs(phiKpM-${Lbmass})>${Lbwindow}||(abs(phiKpM-${Lbmass})<${Lbwindow}&&proton_ProbNNk>proton_ProbNNp)" \
             ${plotoptions} \
             --plot "latex/figs/prodPNNkp_ROC_best" \
             --blurb "#splitline{P_{NN}^{K}(1 #minus P_{NN}^{p}) < [$vmin, $vmax] on best proton}{#scale[0.8]{in |m(#phi Kp) #minus m(#Lambda_{b})| < ${Lbwindow} MeV using best proton}}"
bothcut=()
compcuts=()
for i in 0 1
do
  bothcut+=("(abs(${Lbmasses[$i]}-${Lbmass})>${Lbwindow}||(abs(${Lbmasses[$i]}-${Lbmass})<${Lbwindow}&&${kaons[$i]}_ProbNNk*(1-${kaons[$i]}_ProbNNp)>{}))")
  compcuts+=("(abs(${Lbmasses[$i]}-${Lbmass})>${Lbwindow}||(abs(${Lbmasses[$i]}-${Lbmass})<${Lbwindow}&&${kaons[$i]}_ProbNNk>${kaons[$i]}_ProbNNp))")
done
bin/ROCcurve ${Lbdata} \
             --expr "${bothcut[0]}&&${bothcut[1]}" \
             --comp "${compcuts[0]}&&${compcuts[1]}" \
             ${plotoptions} \
             --plot "latex/figs/prodPNNkp_ROC_both" \
             --blurb "#splitline{P_{NN}^{K}(1 #minus P_{NN}^{p}) < [$vmin, $vmax] on proton track}{#scale[0.8]{#splitline{in |m(#phi K^{#minus}p) #minus m(#Lambda_{b})| < ${Lbwindow} MeV}{or |m(#phi K^{#plus}#bar{p}) #minus m(#bar{#Lambda}_{b})| < ${Lbwindow} MeV}}}"

######################################################################################

inwindowcut=""
compinwindowcut=""
for particle in ${kaons[@]}
do
  inwindowcut+="&&(${particle}_ProbNNk*(1-${particle}_ProbNNpi))>{}"
  compinwindowcut+="&&(${particle}_ProbNNk>${particle}_ProbNNpi)"
done
bin/ROCcurve ${Bddata} \
             --expr "abs(phiKpiM-${Bdmass})>${Bdwindow}||(abs(phiKpiM-${Bdmass})<${Bdwindow}${inwindowcut})" \
             --comp "abs(phiKpiM-${Bdmass})>${Bdwindow}||(abs(phiKpiM-${Bdmass})<${Bdwindow}${compinwindowcut})" \
             ${plotoptions} \
             --plot "latex/figs/prodPNNkpi_ROC" \
             --blurb "#splitline{P_{NN}^{K}(1 #minus P_{NN}^{#pi}) < [$vmin, $vmax] on both tracks}{#scale[0.8]{in |m(#phi K#pi) #minus m(B^{0})| < ${Bdwindow} MeV using best pion}}"

bin/ROCcurve ${Bddata} \
             --expr "abs(phiKpiM-${Bdmass})>${Bdwindow}||(abs(phiKpiM-${Bdmass})<${Bdwindow}&&pion_ProbNNk*(1-pion_ProbNNpi)>{})" \
             --comp "abs(phiKpiM-${Bdmass})>${Bdwindow}||(abs(phiKpiM-${Bdmass})<${Bdwindow}&&pion_ProbNNk>pion_ProbNNpi)" \
             ${plotoptions} \
             --plot "latex/figs/prodPNNkpi_ROC_best" \
             --blurb "#splitline{P_{NN}^{K}(1 #minus P_{NN}^{#pi}) < [$vmin, $vmax] on best pion}{#scale[0.8]{in |m(#phi K#pi) #minus m(B^{0})| < ${Bdwindow} MeV using best pion}}"
bothcut=()
compcuts=()
for i in 0 1
do
  bothcut+=("(abs(${Bdmasses[$i]}-${Bdmass})>${Bdwindow}||(abs(${Bdmasses[$i]}-${Bdmass})<${Bdwindow}&&${kaons[$i]}_ProbNNk*(1-${kaons[$i]}_ProbNNpi)>{}))")
  compcuts+=("(abs(${Bdmasses[$i]}-${Bdmass})>${Bdwindow}||(abs(${Bdmasses[$i]}-${Bdmass})<${Bdwindow}&&${kaons[$i]}_ProbNNk>${kaons[$i]}_ProbNNpi))")
done
bin/ROCcurve ${Bddata} \
             --expr "${bothcut[0]}&&${bothcut[1]}" \
             --comp "${compcuts[0]}&&${compcuts[1]}" \
             ${plotoptions} \
             --plot "latex/figs/prodPNNkpi_ROC_both" \
             --blurb "#splitline{P_{NN}^{K}(1 #minus P_{NN}^{#pi}) < [$vmin, $vmax] on pion track}{#scale[0.8]{#splitline{in |m(#phi K^{#minus} #pi^{#plus}) #minus m(#bar{B}^{0})| < ${Bdwindow} MeV}{or |m(#phi K^{#plus}#pi^{#minus}) #minus m(B^{0})| < ${Bdwindow} MeV}}}"

