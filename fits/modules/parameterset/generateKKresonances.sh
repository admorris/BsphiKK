#!/bin/bash
function newKK
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
	./newresonance.sh ${name} ${mass} ${mloerr} ${muperr} ${width} ${wloerr} ${wuperr} > resonances/${name}_fixed.xml
	./newamplitude.sh ${name} ${spin} > amplitudes/${name}_float.xml
	./newfraction.sh ${name} ${spin} > fractions/${name}_float.xml
	./newconstraint.sh ${name} ${mass} ${mloerr} ${muperr} ${width} ${wloerr} ${wuperr} > ../constraintfunction/${name}_constraint.xml
}
#       Name            J       mass    +       −       width   +       −
newKK   ftwo1270        2       1.2755  0.0008  0.0008  0.1867  0.0022  0.0025
newKK   atwo1320        2       1.3181  0.0007  0.0007  0.107   0.005   0.005
newKK   fzero1370       0       1.350   0.150   0.150   0.350   0.150   0.150
#newKK   ftwo1430        2       1.430   ?       ?       ?       ?       ?
newKK   azero1450       0       1.474   0.019   0.019   0.265   0.013   0.013
newKK   fzero1500       0       1.504   0.006   0.006   0.109   0.007   0.007
newKK   ftwo1565        2       1.562   0.013   0.013   0.134   0.008   0.008
newKK   ftwo1640        2       1.639   0.006   0.006   0.099   0.060   0.040
newKK   phi1680         1       1.680   0.020   0.020   0.159   0.050   0.050
newKK   rhothree1690    3       1.696   0.004   0.004   0.161   0.010   0.010
newKK   rho1700         1       1.720   0.020   0.020   0.250   0.100   0.100
newKK   atwo1700        2       1.732   0.016   0.016   0.194   0.040   0.040
newKK   fzero1710       0       1.723   0.006   0.005   0.139   0.008   0.008
newKK   ftwo1810        2       1.815   0.012   0.012   0.197   0.022   0.022
exit 0

