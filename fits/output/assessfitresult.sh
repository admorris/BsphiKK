#!/bin/bash
function getnumber() # return a number from something of the form "<string>:<whitespace><float>"
{
	grep "$1:" $2 | sed "s/.*${1}:\s*//" | sed -r 's/([0-9\.]*).*/\1/'
}
cd $1
cwd=$(pwd)
echo "folder & fit status & NLL & AIC & BIC & Chi^2 & \$f_2^\\prime\$ fraction & \$f_2^\\prime\$ \$f_L\$\\\\"
for folder in $(ls | grep FitResult)
do
	cd $cwd/$folder
	file=$(ls RapidFitOutput-*.log | tail -1)
	chisq=$(getnumber "Chi\^2\/ndof " $file | tr '\n' ' ')
	nll=$(getnumber "NLL" $file)
	aic=$(getnumber "AIC" $file)
	bic=$(getnumber "BIC" $file)
	ftwopfrac=$(getnumber "1ftwop1525LHCb(2,BW)           " $file)
	ftwopfL=$(getnumber "ftwop1525LHCb\_Azerosq     " $file)
	status=$(getnumber "Status" $file | tail -1)
	echo "${folder} & ${status} & ${nll} & ${aic} & ${bic} & ${chisq} & ${ftwopfrac} & ${ftwopfL} \\\\"
done

