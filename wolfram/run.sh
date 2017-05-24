#!/bin/bash
filename=`echo "$1" | sed 's/\.[A-Za-z]*$//'`
if [ $# == 1 ]
then
	cat <<-EOF > $filename.tex
	\documentclass{article}
	\usepackage[a3paper,margin=1in,landscape]{geometry}
	\usepackage{amsmath}
	\begin{document}
	\begin{eqnarray*}
	EOF
	math -noprompt -run "<< $1" | sed 's/\\left\\left/\\left/g' | sed 's/\\right\\right/\\right/g' >> $filename.tex
	cat <<-EOF >> $filename.tex
	\end{eqnarray*}
	\end{document}
	EOF
	pdflatex -interaction=batchmode $filename.tex
else
	echo "Please provide exactly 1 argument: the name of a Wolfram script which produces LaTeX output"
fi
rm -f $filename.aux $filename.log $filename.nav $filename.snm $filename.toc $filename.out
