#!/bin/bash
# Turn the names of fit parameters into symbols
function replace()
{
	for filename in JustFit ToShare FullTable SimpleTable MinimalTable CorrelationMatrix
	do
		sed -i -r "s/$1/$2/g" $filename.tex
	done
}

replace '\\text\{([^}]*)\}' '\1'
replace '\\_' ' \\;'
replace 'phi' '\\phi('
replace 'fzero' 'f_0('
replace 'ftwop' 'f_2^\\prime('
replace 'ftwo' 'f_2('
replace 'LHCb' ''
replace 'Azero' '|A_0|'
replace 'Aplus' '|A_+|'
replace 'Aminus' '|A_-|'
replace 'deltazero' '\\delta_0'
replace 'deltaplus' '\\delta_+'
replace 'deltaminus' '\\delta_-'
replace 'fraction' 'f'
replace 'mass' 'm'
replace 'width' '\\Gamma'
replace 'G([HL])' '\\Gamma_\1'
replace 'nonres' '\\text{NR}'
replace 'signal' '\\text{sig}'
replace 'combinatorial' '\\text{bkg}'
replace '\(1020' ''
replace '\(1525' ''
replace '\(([0-9]+) ' '\\text{\\scriptsize(\1)} '
replace '(..)BFradius' 'r_{\1}'
replace 'gpipi' 'g_{\\pi\\pi}'
replace 'Rg' 'R_g'
replace 'thraccscale' 'T_'
replace 'mKKres \\;sigmazero' '\\sigma_0'
#sed -i 's/10pt/8pt/' main.tex
sed -i 's/a4paper/a3paper/' main.tex
sed -i '2s;^;\\usepackage{pdflscape}\n;' main.tex
sed -i 's/sidewaystable/landscape/' CorrelationMatrix.tex
sed -i 's/landscape}\[h\]/landscape}/' CorrelationMatrix.tex
pdflatex main

