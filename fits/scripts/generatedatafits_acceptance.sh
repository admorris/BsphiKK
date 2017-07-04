#!/bin/bash
CWD=$(pwd)
#  protect against empty string
if [ -z "${CWD}" ]
then
	exit 1
fi
bindir="${CWD}/../bin"
srcdir="${CWD}/src/datafits/acceptance"
best=(nonres fzero980 phi1020 ftwop1525LHCb)
for suffix in "" "_nophi"
do
	mkdir -p ${srcdir}${suffix}
	cd ${srcdir}${suffix}
	${bindir}/PrintSource ${best[@]}
	make -C "${CWD}/modules" -j 4
	cd ${srcdir/src/results}${suffix}
	filename=$(ls -tr *.xml | tail -1)
	for i in $(seq 1 63)
	do
		newXMLfile=${filename/./_${i}.}
		sed "s/AcceptanceEntry:0/AcceptanceEntry:${i}/" ${filename} > $newXMLfile
		echo "Building ${newXMLfile}"
	done
	rm -v ${filename}
	cd ${CWD}
done
