#!/bin/bash
function getfrac()
{
	echo $(grep "$1" $2) | sed "s/$1 //"
}
Nphi=$(getfrac "1phi1020(1,BW):" $1)
BFphitoKK=""
Nftwop=$(getfrac "1ftwop1525(2,BW):" $1)
BFftwoptoKK=""
if [[ "$Nphi" == "" || "$Nftwop" == "" ]]
then
	exit 0
fi
echo "ϕ frac: $Nphi"
echo "f2´ frac: $Nftwop"
echo "($Nftwop/$Nphi)*()" | bc -l

