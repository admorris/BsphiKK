#!/bin/bash
recentdir=$(find . -type d -name "RapidFitOutput_*" | tail -1)
find $recentdir -type f -name "Overlay_*_All_Data.pdf" -exec cp {} ./slides \;
cd slides
pdflatex slide.tex
cd ..
