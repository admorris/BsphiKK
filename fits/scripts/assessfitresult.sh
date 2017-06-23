#!/bin/bash
function getnumber() # return a number from something of the form "<string>:<whitespace><float>"
{
	returnval=$(grep "$1\s*:" $2 | sed "s/.*${1}\s*:\s*//" | sed -r 's/([-0-9\.e\+]*).*/\1/' | tail -1)
	if [[ -z $returnval || $returnval == *"e-"* ]]
	then
		returnval="0"
	fi
	echo ${returnval/ /}
}
function getnumbers()
{
	returnval=$(grep "$1\s*:" $2 | sed "s/.*${1}\s*:\s*//" | sed -r 's/([0-9\.e\-\+]*).*/\1/' | tail -${3} | tr '\n' ' ')
	declare format
	for i in $(seq ${3})
	do
		format="$format %.3f"
	done
	printf "$format" $returnval
}
function paramsatlimit()
{
	echo $(grep "Caution: Parameter" $1 | sed -r 's/Caution: Parameter (\S*) .*$/\1/g' | tr '\n' ', ')
}
function parseparticlenames()
{
	echo $1 | sed 's/_/ /g' | sed 's/phi/phi /g' | sed 's/fzero/f0 /g' | sed "s/ftwop/f2' /g" | sed "s/ftwo/f2 /g" | sed 's/LHCb//g' | sed -r 's/ ([0-9]+)/(\1)/g' | sed "s/zero/0/g" | sed "s/plus/+/g" | sed "s/minus/-/g" | sed -r "s/(A.)sq/|\1|^2/g" | sed "s/delta/d/g" | sed "s/,$//g" | sed "s/,/, /g" | sed "s/fraction/f/g" | sed "s/nonres/NR/g"
}
cd $1
cwd=$(pwd)
printf '%-68s | %-1s | %-7s | %-7s | %-7s | %24s | %-5s | %-5s | %-5s | %-7s | %-4s | %-6s | Parameters at limit\n' "folder" "S" "NLL" "AIC" "BIC" "Projection χ^2/ndof" "NR f" "f0 f" "phi f" "phi F_0" "f2' f" "f2' F_0"
for folder in $(ls | grep FitResult)
do
	cd $cwd/$folder
	for file in $(ls | grep "RapidFitOutput-.*\.log" | tail $2)
	do
		chisq=$(getnumbers "Chi\^2\/ndof " $file 4 | tr '\n' ' ')
		nll=$(getnumber "NLL" $file)
		aic=$(getnumber "AIC" $file)
		bic=$(getnumber "BIC" $file)
		NRfrac=$(getnumber "1nonres" $file)
		fzfrac=$(getnumber "1fzero980" $file)
		phifrac=$(getnumber   "1phi1020" $file)
		phifL=$(getnumber   "phi1020\_Azerosq" $file)
		ftwopfrac=$(getnumber "1ftwop1525LHCb" $file)
		ftwopfL=$(getnumber   "ftwop1525LHCb\_Azerosq" $file)
		status=$(getnumber "Status" $file | tail -1)
		printf '%-68s | %1d | %7.1f | %7.1f | %7.1f | %5.3f %5.3f %5.3f %5.3f | %5.3f | %5.3f | %5.3f | %7.3f | %5.3f | %7.3f | ' \
		"$(parseparticlenames ${folder/FitResult_/})" ${status} ${nll} ${aic} ${bic} ${chisq} ${NRfrac} ${fzfrac} ${phifrac} ${phifL} ${ftwopfrac} ${ftwopfL}
		echo $(parseparticlenames "$(paramsatlimit $file)")
	done
done

