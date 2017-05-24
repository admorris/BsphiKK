#!/bin/bash
source cuts.sh
function plotstripcut()
{
  particle=$1
  branch=$2
  title=$3
  unit=$4
  lolim=$5
  uplim=$6
  cutv=$7
  nbins=$8
  ../bin/PlotBranch \
    -F ../ntuples/BsphiKK_data_mvaVars_vetoes.root \
    -C "abs(KK_M-${phimass})<$phiwindow" \
    -B "${particle}_$branch" \
    -T "$title" \
    -U "$unit" \
    -O ../latex/figs/strip_${branch}_${particle} \
    -l $lolim \
    -u $uplim \
    -b $nbins \
    --lineat $cutv &
}

#StdNoPIDsPions               : (PT>250*MeV) & (MIPCHI2DV(PRIMARY) > 4.)
#TrackListBsPhiRho            : (TRGHOSTPROB < 0.5) & (PT>500.*MeV) & (TRCHI2DOF < 3.5) & (MIPCHI2DV(PRIMARY) > 16)
#DiTracksForCharmlessBBsPhiRho: (APT> 900.0 *MeV) & (AP> 1.0 *GeV) & in_range( 0.0 ,AM, 4000.0 )
#                   Mother cut: (MIPCHI2DV(PRIMARY) > 16)

#StdTightKaons                : ["RequiresDet='RICH' CombDLL(k-pi)>'0.0'"]
#StdTightPhi2KK               : (ADAMASS('phi(1020)')<30*MeV) & (ADOCACHI2CUT(30, ''))
#    Mother cut               : (VFASPF(VCHI2) < 25.0)
#KKTracksForCharmlessBBsPhiRho: (MIPCHI2DV(PRIMARY) > 10) & (ADMASS('phi(1020)')< 25.0 *MeV) & (PT> 900.0 *MeV) & (P> 1.0 *GeV)

#BsPhiRhoLine                 : in_range( 4800.0 ,AM, 5600.0 )
#  Mother cut                 : (MIPCHI2DV(PRIMARY) < 20) & (VFASPF(VCHI2/VDOF) < 9.0 )

# Kaons
# - pT
# - DLL(K−π)
# - IP χ²
# - Track χ²
# - Ghost Prob
for track in "Kplus" "Kminus" "Kplus0" "Kminus0"
do #           Particle   Branch            Title                                   Unit         min  max  cut bins
  plotstripcut "${track}" "PT"              "p_{T}"                                 "MeV/#it{c}"   0 2000  500  200
  plotstripcut "${track}" "PIDK"            "DLL(#it{K#minus#pi})"                  ""           -10   80    0   90
  plotstripcut "${track}" "IPCHI2_OWNPV"    "#chi^{2}_{IP}"                         ""             0  100   16  100
  plotstripcut "${track}" "TRACK_CHI2NDOF"  "#chi^{2}_{Track}/ndof"                 ""             0    5  3.5   50
  plotstripcut "${track}" "TRACK_GhostProb" "Ghost prob"                            ""             0    1  0.3  100
done
# ϕ and KK
# - pT
# - p
# - IP χ²
# - DOCA χ²
# - Vtx χ²
# - Mass
for KKres in "phi_1020" "KK"
do #           Particle   Branch            Title                                   Unit              min    max  cut bins
  plotstripcut "${KKres}" "PT"              "p_{T}"                                 "MeV/#it{c}"        0   5000  900  100
  plotstripcut "${KKres}" "P"               "p"                                     "MeV/#it{c}"        0  20000 1000  100
  plotstripcut "${KKres}" "IPCHI2_OWNPV"    "#chi^{2}_{IP}"                         ""                  0    512   16   64
  plotstripcut "${KKres}" "ENDVERTEX_CHI2"  "#chi^{2}_{Vtx}"                        ""                  0     30   25   30
  plotstripcut "${KKres}" "M"               "#it{m}(#it{K}^{#plus}#it{K}^{#minus})" "MeV/#it{c}^{2}" 1000   1050    0   50
done
wait

