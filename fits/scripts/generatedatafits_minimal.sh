#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/minimal_extra"
best=(nonres fzero980 phi1020 ftwop1525LHCb)
mkdir -p ${srcdir}
cd ${srcdir}
${bindir}/PrintSource ${best[@]}
cd ${CWD}
for extra in "fzero1370LHCb" "fzero1500LHCb" "ftwo1640" "phi1680" "fzero1710" "ftwo1750"
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

