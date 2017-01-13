#!/bin/bash
function newKK()
{
	echo "Building ${name}"
	name=$1
	spin=$2
	mass=$3
	muperr=$4
	mloerr=$5
	width=$6
	wuperr=$7
	wloerr=$8
	./newresonance.sh ${name} ${mass} ${width} > resonances/${name}_float.xml
	cp resonances/${name}_float.xml resonances/${name}_fixed.xml
	sed -i 's/Float/Fixed/' resonances/${name}_fixed.xml
	./newamplitude.sh ${name} ${spin} > amplitudes/${name}_float.xml
	cp amplitudes/${name}_float.xml amplitudes/${name}_fixed.xml
	sed -i 's/Float/Fixed/' amplitudes/${name}_fixed.xml
	./newfraction.sh ${name} ${spin} > fractions/${name}_float.xml
	./newconstraint.sh ${name} ${mass} ${mloerr} ${muperr} ${width} ${wloerr} ${wuperr} > ../constraintfunction/${name}_constraint.xml
}
#       Name            J       mass    +       −       width   +       −
# Note: f0(980) added by hand because Flatte
newKK   phi1020         1    1.019461 0.000019 0.000019 0.004266 0.000031 0.000031
newKK   ftwo1270        2       1.2755  0.0008  0.0008  0.1867  0.0022  0.0025
newKK   atwo1320        2       1.3181  0.0007  0.0007  0.107   0.005   0.005
newKK   fzero1370       0       1.350   0.150   0.150   0.350   0.150   0.150
newKK   fzero1370LHCb   0       1.475   0.006   0.006   0.113   0.011   0.011
newKK   azero1450       0       1.474   0.019   0.019   0.265   0.013   0.013
newKK   fzero1500       0       1.504   0.006   0.006   0.109   0.007   0.007
newKK   ftwop1525       2       1.525   0.005   0.005   0.073   0.006   0.005
newKK   ftwop1525LHCb   2       1.522   0.006   0.003   0.084   0.012   0.008
newKK   ftwo1565        2       1.562   0.013   0.013   0.134   0.008   0.008
newKK   ftwo1640        2       1.639   0.006   0.006   0.099   0.060   0.040
newKK   phi1680         1       1.680   0.020   0.020   0.159   0.050   0.050
newKK   rhothree1690    3       1.696   0.004   0.004   0.161   0.010   0.010
newKK   rho1700         1       1.720   0.020   0.020   0.250   0.100   0.100
newKK   atwo1700        2       1.732   0.016   0.016   0.194   0.040   0.040
newKK   fzero1710       0       1.723   0.006   0.005   0.139   0.008   0.008
newKK   ftwo1810        2       1.815   0.012   0.012   0.197   0.022   0.022
# Custom styles for the phi and f2'
for file in $(ls fractions/phi1020* fractions/ftwop1525*)
do
	sed -i 's/style: 1/style: 9/' $file
	sed -i 's/width: 1/width 2/' $file
	sed -i 's/colour: 1/colour: 28/' $file
done
sed -i 's/colour: [0-9]*/colour: 6/' fractions/phi1020*
exit 0

