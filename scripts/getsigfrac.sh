#!/bin/bash
function ExtractNumber()
{
	echo `../bin/PrintResult $1 "$2" | grep "^$3.*$" | sed -r "s/^$3\s*:\s*([0-9\.]*.*$)/\1/"`
}
for table in `ls tables/Bsmassfit_*.csv`
do
	S=`ExtractNumber $table BsmassfitmvacutSignalNtwosigma rawvalue`
	dS=`ExtractNumber $table BsmassfitmvacutSignalNtwosigma rawerror`
	B=`ExtractNumber $table BsmassfitmvacutCombinatorialNtwosigma rawvalue`
	dB=`ExtractNumber $table BsmassfitmvacutCombinatorialNtwosigma rawerror`
	mass=`echo $table | sed 's/\.csv//' | sed 's/tables\/Bsmassfit\_//'`
	../bin/sigfraccalc $mass $S $dS $B $dB
done

# Overwrite the 1800 fraction using a fit includling peaking backgrounds
table="tables/MassFits.csv"
S=`ExtractNumber $table BsmassfitmvacutpkgbkgsBSignalNtwosigma rawvalue`
dS=`ExtractNumber $table BsmassfitmvacutpkgbkgsBSignalNtwosigma rawerror`
B=`ExtractNumber $table BsmassfitmvacutpkgbkgsBCombinatorialNtwosigma rawvalue`
dB=`ExtractNumber $table BsmassfitmvacutpkgbkgsBCombinatorialNtwosigma rawerror`
PB1=`ExtractNumber $table BsmassfitmvacutpkgbkgsBpeakingANtwosigma rawvalue`
dPB1=`ExtractNumber $table BsmassfitmvacutpkgbkgsBpeakingANtwosigma rawerror`
PB2=`ExtractNumber $table BsmassfitmvacutpkgbkgsBpeakingBNtwosigma rawvalue`
dPB2=`ExtractNumber $table BsmassfitmvacutpkgbkgsBpeakingBNtwosigma rawerror`
B=`echo "$B+$PB1+$PB2" | bc -l`
dB=`echo "sqrt($dB^2+$dPB1^2+$dPB2^2)" | bc -l`
../bin/sigfraccalc 1800 $S $dS $B $dB

mv -v signal_fraction_*_fixed.xml ../fits/modules/parameterset/

