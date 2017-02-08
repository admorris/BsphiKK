#!/bin/bash
# Self-documenting the argument set
name=$1
spin=$2
phase=$3
# Generate the XML
if [ ${spin} == "0" ]
then
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltazero</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-3.7</Minimum>"
	echo "	<Maximum>3.7</Maximum>"
	echo "	<Type>Float</Type> #phase"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
else
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_Aperpsq</Name>"
	echo "	<Value>0.333</Value>"
	echo "	<Minimum>0.0</Minimum>"
	echo "	<Maximum>1.0</Maximum>"
	echo "	<Type>Float</Type>"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_Azerosq</Name>"
	echo "	<Value>0.333</Value>"
	echo "	<Minimum>0.0</Minimum>"
	echo "	<Maximum>1.0</Maximum>"
	echo "	<Type>Float</Type>"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltaperp</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-3.7</Minimum>"
	echo "	<Maximum>3.7</Maximum>"
	echo "	<Type>Float</Type> #phase"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltazero</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-3.7</Minimum>"
	echo "	<Maximum>3.7</Maximum>"
	echo "	<Type>Float</Type> #phase"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
	echo "<PhysicsParameter>"
	echo "	<Name>${name}_deltapara</Name>"
	echo "	<Value>${phase}</Value>"
	echo "	<Minimum>-3.7</Minimum>"
	echo "	<Maximum>3.7</Maximum>"
	echo "	<Type>Float</Type> #phase"
	echo "	<Unit>Unitless</Unit>"
	echo "</PhysicsParameter>"
fi
exit 0

