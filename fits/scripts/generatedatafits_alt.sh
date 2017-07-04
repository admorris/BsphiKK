#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/alt"
best=(nonres fzero980 phi1020 ftwop1525LHCb)
mkdir -p ${srcdir}
mkdir -p ${srcdir}_nophi
for extra in "altbarrier" "altflatte" "altftwop" "splitbyyear" "splitbytrigger" "splitbymagnet"
do
	cd ${srcdir}
	${bindir}/PrintSource ${best[@]} ${extra}
	cd ${srcdir}_nophi
	${bindir}/PrintSource ${best[@]} ${extra} nophi
	cd ${CWD}
done

