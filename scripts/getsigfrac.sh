#!/bin/bash
function ExtractNumber()
{
	echo `../bin/PrintResult tables/MassFits.csv "$1" | grep "^$2.*$" | sed -r "s/^$2\s*:\s*([0-9\.]*.*$)/\1/"`
}
S=`ExtractNumber BsmassfitmvacutpkgbkgsBSignalNtwosigma rawvalue`
dS=`ExtractNumber BsmassfitmvacutpkgbkgsBSignalNtwosigma rawerror`
B=`ExtractNumber BsmassfitmvacutpkgbkgsBCombinatorialNtwosigma rawvalue`
dB=`ExtractNumber BsmassfitmvacutpkgbkgsBCombinatorialNtwosigma rawerror`
PB1=`ExtractNumber BsmassfitmvacutpkgbkgsBpeakingANtwosigma rawvalue`
dPB1=`ExtractNumber BsmassfitmvacutpkgbkgsBpeakingANtwosigma rawerror`
PB2=`ExtractNumber BsmassfitmvacutpkgbkgsBpeakingBNtwosigma rawvalue`
dPB2=`ExtractNumber BsmassfitmvacutpkgbkgsBpeakingBNtwosigma rawerror`
B=`echo "$B+$PB1+$PB2" | bc -l`
dB=`echo "sqrt($dB^2+$dPB1^2+$dPB2^2)" | bc -l`
../bin/sigfraccalc 1800 $S $dS $B $dB
mv signal_fraction_1800_fixed.xml ../fits/modules/parameterset/

