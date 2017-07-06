#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/acceptance"
best=(nonres fzero980 phi1020 ftwop1525LHCb phi1680)
mkdir -p ${srcdir}
cd ${srcdir}
${bindir}/PrintSource ${best[@]}
