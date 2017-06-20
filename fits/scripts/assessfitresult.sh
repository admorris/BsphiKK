#!/bin/bash
function getnumber() # return a number from something of the form "<string>:<whitespace><float>"
{
	grep "$1:" $2 | sed "s/.*${1}:\s*//" | sed -r 's/([0-9\.]*).*/\1/' | tail -1
}
cd $1
cwd=$(pwd)
printf '%-70s & %-6s & %-12s & %-12s & %-12s & %-12s & %-20s & %-28s & %-20s \\\\\n' "folder" "status" "NLL" "AIC" "BIC" "Chi^2" "\$\\phi\\phi\$ fraction" "\$\\phi f_2^\\prime\$ fraction" "\$f_2^\\prime\$ \$f_L\$"
for folder in $(ls | grep FitResult)
do
	cd $cwd/$folder
	file=$(ls RapidFitOutput-*.log | tail -1)
	chisq=$(getnumber "Chi\^2\/ndof " $file | tr '\n' ' ')
	nll=$(getnumber "NLL" $file)
	aic=$(getnumber "AIC" $file)
	bic=$(getnumber "BIC" $file)
	phifrac=$(getnumber   "1phi1020(1,BW)                 " $file)
	ftwopfrac=$(getnumber "1ftwop1525LHCb(2,BW)           " $file)
	ftwopfL=$(getnumber   "ftwop1525LHCb\_Azerosq     " $file)
	status=$(getnumber "Status" $file | tail -1)
	#echo "${folder/FitResult_/} & ${status} & ${nll} & ${aic} & ${bic} & ${chisq} & ${phifrac} & ${ftwopfrac} & ${ftwopfL} \\\\"
	printf '%-70s & %-6s & %-12s & %-12s & %-12s & %-12s & %-20s & %-28s & %-20s \\\\\n' ${folder/FitResult_/} ${status} ${nll} ${aic} ${bic} "${chisq}" ${phifrac} ${ftwopfrac} ${ftwopfL}
done

