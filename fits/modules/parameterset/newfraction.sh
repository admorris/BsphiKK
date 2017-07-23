#!/bin/bash
# Self-documenting the argument set
name1=$1
name2=$2
spin1=$3
spin2=$4
shape1=$5
shape2=$6
sortedname=($(
for element in ${name1} ${name2}
do
    echo "$element"
done | sort))
# Generate the XML
echo "# shape: ${name1},${name2}(${spin1}${spin2}${shape1}${shape2})"
echo "# style: 1"
echo "# width: 1"
echo "# colour: 1"
echo "<PhysicsParameter>"
echo "	<Name>${sortedname[0]}_${sortedname[1]}_fraction</Name>"
echo "	<Value>0.01</Value>"
echo "	<Minimum>0.0</Minimum>"
echo "	<Maximum>10.0</Maximum>"
echo "	<Type>Float</Type>"
echo "	<Unit>Unitless</Unit>"
echo "</PhysicsParameter>"
exit 0

