#!/bin/bash
# Self-documenting the argument set
name=$1
spin=$2
shape=$3
# Generate the XML
echo "# shape: spin-${spin} ${shape}"
echo "# style: 1"
echo "# width: 1"
echo "# colour: 1"
echo "<PhysicsParameter>"
echo "	<Name>${name}_fraction</Name>"
echo "	<Value>0.01</Value>"
echo "	<Minimum>0.0</Minimum>"
echo "	<Maximum>10.0</Maximum>"
echo "	<Type>Float</Type>"
echo "	<Unit>Unitless</Unit>"
echo "</PhysicsParameter>"
exit 0

