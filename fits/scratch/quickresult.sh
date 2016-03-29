#!/bin/bash
recentdir=$(find . -type d -name "RapidFitOutput_*" | sort | tail -1)
find $recentdir -type f -name "Overlay_*_All_Data.pdf" -exec cp -v {} ./slides \;
cd slides
pdflatex -interaction=batchmode slide.tex
cd ..
