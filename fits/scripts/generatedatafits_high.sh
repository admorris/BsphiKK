#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/highmass"
best=(nonres fzero980 phi1020 ftwop1525LHCb)
mkdir -p ${srcdir}
for extra in "ftwo1640" "phi1680" "fzero1710" "ftwo1750"
do
	options=()
	if [[ "${extra}" != *"zero"* ]]
	then
		options+=("_longitudinal" "_transverse")
	fi
	for option in "" ${options[@]}
	do
		cd ${srcdir}
		${bindir}/PrintSource ${best[@]} ${extra}${option}
		cd ${CWD}
	done
done

