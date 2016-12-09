#!/bin/bash
if [ "$1" == "" ]
then
  recentdir=$(find . -type d -name "RapidFitOutput_*" | sort | tail -1)
else
  recentdir=$1
fi
if [ "$recentdir" == "" ]
then
  echo "Can't find a RapidFitOutput dir. Please provide it as an argument."
  exit 0
fi
mkdir -p tmp
find $recentdir -type f -name "Overlay_*_All_Data.pdf" -exec cp -v --backup=t {} ./tmp \;
cd tmp
cat <<EOF > projections.tex
\documentclass[16pt]{beamer}
\mode<presentation>
\setbeamertemplate{navigation symbols}{}
\begin{document}
  \begin{frame}
    \centering
EOF
if [ -e Overlay_mKK_All_Data.pdf ]
then
  echo "    \includegraphics[width=0.5\textwidth]{Overlay_mKK_All_Data.pdf}" >> projections.tex
fi
cat <<EOF >> projections.tex
    \includegraphics[width=0.5\textwidth]{Overlay_phi_All_Data.pdf}\\
    \includegraphics[width=0.5\textwidth]{Overlay_ctheta_1_All_Data.pdf}
    \includegraphics[width=0.5\textwidth]{Overlay_ctheta_2_All_Data.pdf}
  \end{frame}
\end{document}
EOF
pdflatex -interaction=batchmode projections.tex
cd ..
mv -v tmp/projections.pdf $recentdir
rm -rf tmp
