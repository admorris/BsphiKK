#!/bin/bash
# Self-documenting the argument set
name=$1
mass=$2
mloerr=$3
muperr=$4
width=$5
wloerr=$6
wuperr=$7
# Work out what to put in the comment
if [ "${mloerr}" == "${muperr}" ]
then
	merr="±${muperr}"
else
	merr="+${muperr}−${mloerr}"
fi
if [ "${wloerr}" == "${wuperr}" ]
then
	werr="±${wuperr}"
else
	werr="+${wuperr}−${wloerr}"
fi
# Generate the XML
echo "<PhysicsParameter>"
echo "	<Name>${name}_mass</Name>"
echo "	<Value>${mass}</Value> # PDG: ${mass}${merr}"
mmin=$(echo "${mass}-${mloerr}" | bc -l)
mmax=$(echo "${mass}+${muperr}" | bc -l)
echo "	<Minimum>${mmin}</Minimum>"
echo "	<Maximum>${mmax}</Maximum>"
echo "	<Type>Fixed</Type>"
echo "	<Unit>GeV</Unit>"
echo "</PhysicsParameter>"
echo "<PhysicsParameter>"
echo "	<Name>${name}_width</Name> # PDG: ${width}${werr}"
echo "	<Value>${width}</Value>"
wmin=$(echo "${width}-${wloerr}" | bc -l)
wmax=$(echo "${width}+${wuperr}" | bc -l)
echo "	<Minimum>${wmin}</Minimum>"
echo "	<Maximum>${wmax}</Maximum>"
echo "	<Type>Fixed</Type>"
echo "	<Unit>GeV</Unit>"
echo "</PhysicsParameter>"
exit 0

