#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/significance"
best=(nonres fzero980 phi1020 ftwop1525LHCb phi1680)
mkdir -p ${srcdir}
for toremove in ${best[@]}
do
	if [[ "${toremove}" != *"phi1020"* ]]
	then
		resonances=()
		for res in ${best[@]}
		do
			if [[ "${toremove}" != *"${res}"* ]]
			then
				resonances+=(${res})
			fi
		done
		cd ${srcdir}
		${bindir}/PrintSource ${resonances[@]}
		cd ${CWD}
	fi
done

