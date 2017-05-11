#!/bin/bash
function expectedyield()
{
	NSigFit=$1 # Number of signal events in data
	NSigGen=$2 # Number of generated signal events in MC, accounting for generator-level efficiency
	NSigSel=$3 # Number of selected signal events in MC
	NBkgGen=$4 # Number of generated background events in MC, accounting for generator-level efficiency
	NBkgSel=$5 # Number of selected background events in MC
	ExtFact=$6 # External factors (branching/fragmentation fractions etc)
	echo "$NSigFit*($NSigGen/$NSigSel)*($NBkgSel/$NBkgGen)*$ExtFact" | bc -l
}
# External stuff
BFBsPhiPhi="0.0000184"
BFBdPhiKst="0.00001"
BFLbPhiKp="0.00000518"
BFPhiKK="0.489"
BFKstKpi="0.66666666667"
fdfs=$(echo "1/0.259" | bc -l)
fLbfs=$(echo "0.404*(1-0.031*10.42)/0.134" | bc -l)
# Bs to phi phi
NFitBsPhiPhi=$(../bin/PrintResult tables/MassFits.csv "BsmassfitmvacutlowSignalN" | grep "^rawvalue.*$" | sed -r 's/^rawvalue\s*:\s*([0-9\.]*.*$)/\1/')
NGenBsPhiPhi=$( echo "(511248+513999)/0.167" | bc -l)
NSelBsPhiPhi=$(../bin/GetEntries ../ntuples/Bsphiphi_MC_mvacut.root DecayTree "BCON_KK_M<1800")
# Bd to phi K*
NGenBdPhiKst=$( echo "(2020493+2007492)/0.187" | bc -l)
NSelBdPhiKst=$(../bin/GetEntries ../ntuples/BdphiKst_MC_mvacut.root DecayTree "BCON_KK_M<1800")
# Lb to phi K p
NGenLbPhiKp=$( echo "(528968+523401)/0.184" | bc -l)
NSelLbPhiKp=$(../bin/GetEntries ../ntuples/LbphiKp_MC_mvacut.root DecayTree "BCON_KK_M<1800")
# Calculate expected yields
NExpBdPhiKst=$(expectedyield $NFitBsPhiPhi $NGenBsPhiPhi $NSelBsPhiPhi $NGenBdPhiKst $NSelBdPhiKst $(echo "($BFBdPhiKst/$BFBsPhiPhi)*($BFKstKpi/$BFPhiKK)*$fdfs" | bc -l))
NExpLbPhiKp=$(expectedyield $NFitBsPhiPhi $NGenBsPhiPhi $NSelBsPhiPhi $NGenLbPhiKp $NSelLbPhiKp $(echo "($BFLbPhiKp/$BFBsPhiPhi)*(1/$BFPhiKK)*$fdfs" | bc -l))

echo "Number of expected B⁰→ϕK* events: $NExpBdPhiKst"
echo "Number of expected Λb→ϕKp events: $NExpLbPhiKp"
