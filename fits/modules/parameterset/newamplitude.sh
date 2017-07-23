#!/bin/bash
# Self-documenting the argument set
name1=$1
name2=$2
spin1=$3
spin2=$4
phase=$5
sortedname=($(
for element in ${name1} ${name2}
do
    echo "$element"
done | sort))
name="${sortedname[0]}_${sortedname[1]}"
# Generate the XML
if [[ ${spin1} == "0" || ${spin2} == 0 ]]
then
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltazero</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-3.7</Minimum>"
	echo "	<Maximum>3.7</Maximum>"
	echo "	<Type>Float</Type> #deltazero"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
else
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_Aplus</Name>"
	echo "	<Value>0.577</Value> #Aplus"
	echo "	<Minimum>0.0</Minimum>"
	echo "	<Maximum>1.0</Maximum>"
	echo "	<Type>Float</Type> #Aplus"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_Azero</Name>"
	echo "	<Value>0.577</Value> #Azero"
	echo "	<Minimum>0.0</Minimum>"
	echo "	<Maximum>1.0</Maximum>"
	echo "	<Type>Float</Type> #Azero"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltaplus</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-6.7</Minimum>"
	echo "	<Maximum>6.7</Maximum>"
	echo "	<Type>Float</Type> #deltaplus"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltazero</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-6.7</Minimum>"
	echo "	<Maximum>6.7</Maximum>"
	echo "	<Type>Float</Type> #deltazero"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltaminus</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-6.7</Minimum>"
	echo "	<Maximum>6.7</Maximum>"
	echo "	<Type>Float</Type> #deltaminus"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
fi
exit 0

