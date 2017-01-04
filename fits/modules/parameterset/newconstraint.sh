#!/bin/bash
# Self-documenting the argument set
name=$1
mass=$2
mloerr=$3
muperr=$4
width=$5
wloerr=$6
wuperr=$7
# Calculate width of Gaussian contraint
if [ "${mloerr}" == "${muperr}" ]
then
	merr="${muperr}"
else
	merr=$(echo "(${muperr}+${mloerr})/2" | bc -l)
fi
if [ "${wloerr}" == "${wuperr}" ]
then
	werr="${wuperr}"
else
	werr=$(echo "(${wuperr}+${wloerr})/2" | bc -l)
fi
# Generate the XML
echo "<ExternalConstraint>"
echo "	<Name>${name}_mass</Name>"
echo "	<Value>${mass}</Value>"
echo "	<Minimum>${merr}</Minimum>"
echo "</ExternalConstraint>"
echo "<ExternalConstraint>"
echo "	<Name>${name}_width</Name>"
echo "	<Value>${width}</Value>"
echo "	<Error>${werr}</Error>"
echo "</ExternalConstraint>"
exit 0

