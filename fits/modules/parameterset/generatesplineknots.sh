function newSK()
{
	name=$1
	echo "Building ${name}"
	mass=$2
	./newresonance.sh ${name} ${mass} 0 > resonances/${name}_float.xml
	cp resonances/${name}_float.xml resonances/${name}_fixed.xml
	sed -i 's/Float/Fixed/' resonances/${name}_fixed.xml
	./newamplitude.sh ${name} 0 0.0 > amplitudes/${name}_float.xml
	./newamplitude.sh ${name} 0 3.14159 > amplitudes/${name}_float_pi.xml
	cp amplitudes/${name}_float.xml amplitudes/${name}_fixed_zero.xml
	mv amplitudes/${name}_float_pi.xml amplitudes/${name}_fixed_pi.xml
	sed -i 's/<Type>Float<\/Type> #phase/<Type>Fixed<\/Type> #phase/' amplitudes/${name}_fixed_zero.xml
	sed -i 's/<Type>Float<\/Type> #phase/<Type>Fixed<\/Type> #phase/' amplitudes/${name}_fixed_pi.xml
	./newfraction.sh ${name} 0 "SP" > fractions/${name}_float.xml
}
newSK thresholdknot 0.987354
newSK 1000knot 1.0
newSK 1100knot 1.1
newSK 1200knot 1.2
newSK 1300knot 1.3
newSK 1400knot 1.4
newSK 1500knot 1.5
newSK 1600knot 1.6
newSK 1700knot 1.7
newSK 1800knot 1.8
exit 0

