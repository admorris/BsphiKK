#!/bin/bash
function getnumber() # return a number from something of the form "<string>:<whitespace><float>"
{
	returnval=$(grep "$1\s*:" $2 | sed "s/.*${1}\s*:\s*//" | sed -r 's/([0-9\.]*).*/\1/' | tail -1)
	if [[ -z $returnval ]]
	then
		returnval="0"
	fi
	echo ${returnval/ /}
}
function getnumbers()
{
	returnval=$(grep "$1\s*:" $2 | sed "s/.*${1}\s*:\s*//" | sed -r 's/([0-9\.]*).*/\1/' | tail -${3} | tr '\n' ' ')
	declare format
	for i in $(seq ${3})
	do
		format="$format %.3f"
	done
	printf "$format" $returnval
}
function paramsatlimit()
{
	echo $(grep "Caution: Parameter" $1 | sed -r 's/Caution: Parameter (\S*) .*$/\1/g' | tr '\n' ' ')
}
cd $1
cwd=$(pwd)
printf '%-70s & %-6s & %-6s & %-6s & %-6s & %23s & %-13s & %-20s & %-18s \\\\ %% Parameters at limit\n' "folder" "status" "NLL" "AIC" "BIC" "Projection Chi^2/ndof" "\$\\phi\\phi\$ \\%" "\$\\phi f_2^\\prime\$ \\%" "\$f_2^\\prime\$ \$f_L\$"
for folder in $(ls | grep FitResult)
do
	cd $cwd/$folder
	for file in $(ls | grep "RapidFitOutput-.*\.log" | tail -1)
	do
		chisq=$(getnumbers "Chi\^2\/ndof " $file 4 | tr '\n' ' ')
		nll=$(getnumber "NLL" $file)
		aic=$(getnumber "AIC" $file)
		bic=$(getnumber "BIC" $file)
		phifrac=$(getnumber   "1phi1020" $file)
		ftwopfrac=$(getnumber "1ftwop1525LHCb" $file)
		ftwopfL=$(getnumber   "ftwop1525LHCb\_Azerosq" $file)
		status=$(getnumber "Status" $file | tail -1)
		#echo "${folder/FitResult_/} & ${status} & ${nll} & ${aic} & ${bic} & ${chisq} & ${phifrac} & ${ftwopfrac} & ${ftwopfL} \\\\"
		printf '%-70s & %6d & %6.1f & %6.1f & %6.1f & %5.3f %5.3f %5.3f %5.3f & %13.3f & %20.3f & %18.3f \\\\ %% %-20s \n' ${folder/FitResult_/} ${status} ${nll} ${aic} ${bic} ${chisq} ${phifrac} ${ftwopfrac} ${ftwopfL} "$(paramsatlimit $file)"
	done
done

