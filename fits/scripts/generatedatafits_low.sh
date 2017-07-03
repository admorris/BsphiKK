#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/lowmass"
mkdir -p ${srcdir}
mkdir -p ${srcdir}_nophi
for extra in "" "fzero1370LHCb" "fzero1500LHCb"
do
	cd ${srcdir}
	${bindir}/PrintSource nonres fzero980 phi1020 ${extra} ftwop1525LHCb
	cd ${srcdir}_nophi
	${bindir}/PrintSource nonres fzero980 phi1020 ${extra} ftwop1525LHCb nophi
	cd ${CWD}
done

