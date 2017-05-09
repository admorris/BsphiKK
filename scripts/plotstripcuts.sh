#!/bin/bash
function plotstripcut()
{
  branch=$1
  title=$2
  unit=$3
  lolim=$4
  uplim=$5
  cutv=$6
  ../bin/PlotBranch \
    -F ../ntuples/BsphiKK_data_cuts.root \
    -C "BCON_KK_M<1800" \
    -B "$branch" \
    -T "$title" \
    -U "$unit" \
    -O ../latex/figs/strip_$branch \
    -l $lolim \
    -u $uplim \
    -b 50
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
# - DLLk
# - IPχ²
# - Trackχ²
# - GhostProb

# ϕ and KK
# - pT
# - p
# - IPχ²
# - DOCAχ²
# - Vtxχ²
# - Mass
for track in "Kplus" "Kminus" "Kplus0" "Kminus0"
do
  plotstripcut "${track}_PT"         "p_{T}"                "MeV/#it{c}"  0 2000  500
  plotstripcut "${track}_DLLk"       "DLL(#it{K#minus#pi})" ""          -10   10    0
  plotstripcut "${track}_IPCHISQ"    "#chi^{2}_{IP}"        ""            0  100   16
  plotstripcut "${track}_TRACKCHISQ" "#chi^{2}_{Track}"     ""            0  100  
  plotstripcut "${track}_GHOSTPROB"  "Ghost prob"           ""            0    1  0.5
done
for resonance in "phi_1020" "KK"
do
  plotstripcut "${resonance}_PT"  "p_{T}" "MeV/#it{c}" 0 3000  900
  plotstripcut "${resonance}_P"   "p"     "MeV/#it{c}" 0 3000 1000
done

