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
echo "	<Minimum>$((${mass}-${mloerr}))</Minimum>"
echo "	<Maximum>$((${mass}+${muperr}))</Maximum>"
echo "	<Type>Fixed</Type>"
echo "	<Unit>GeV</Unit>"
echo "</PhysicsParameter>"
echo "<PhysicsParameter>"
echo "	<Name>${name}_width</Name> # PDG: ${width}${werr}"
echo "	<Value>${width}</Value>"
echo "	<Minimum>$((${width}-${wloerr}))</Minimum>"
echo "	<Maximum>$((${width}+${wuperr}))</Maximum>"
echo "	<Type>Fixed</Type>"
echo "	<Unit>GeV</Unit>"
echo "</PhysicsParameter>"
exit 0

