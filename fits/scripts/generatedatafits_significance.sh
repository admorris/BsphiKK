#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/significance"
best=(nonres fzero980 phi1020 ftwop1525LHCb)
mkdir -p ${srcdir}
mkdir -p ${srcdir}_nophi
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
		cd ${srcdir}_nophi
		${bindir}/PrintSource ${resonances[@]} nophi
		cd ${CWD}
	fi
done

