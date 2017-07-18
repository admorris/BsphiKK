#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/1680_extra"
best=(nonres fzero980 phi1020 fzero1500LHCb ftwop1525LHCb phi1680)
mkdir -p ${srcdir}
for extra in "fzero1370LHCb" "ftwo1640" "fzero1710" "ftwo1750"
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

