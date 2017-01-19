#!/bin/bash
# Self-documenting the argument set
name=$1
mass=$2
width=$3
# Generate the XML
echo "<PhysicsParameter>"
echo "	<Name>${name}_mass</Name>"
echo "	<Value>${mass}</Value>"
echo "	<Minimum>0.988</Minimum>"
echo "	<Maximum>2.0</Maximum>"
echo "	<Type>Float</Type>#Please don't float this without Gaussian constraints!"
echo "	<Unit>GeV</Unit>"
echo "</PhysicsParameter>"
echo "<PhysicsParameter>"
echo "	<Name>${name}_width</Name>"
echo "	<Value>${width}</Value>"
wmax=$(echo "1.5*${width}" | bc -l)
echo "	<Minimum>0</Minimum>"
echo "	<Maximum>${wmax}</Maximum>"
echo "	<Type>Float</Type>"
echo "	<Unit>GeV</Unit>"
echo "</PhysicsParameter>"
exit 0

