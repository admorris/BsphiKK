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
suffix=All_Data_wPulls
#suffix=All_Data
#find $recentdir -type f -name "Overlay_*_${suffix}.pdf" -exec cp -v --backup=t {} ./tmp \;
cp -v --backup=t $recentdir/*/Overlay_*_${suffix}.pdf ./tmp
cd tmp
cat <<EOF > projections.tex
\documentclass[16pt]{beamer}
\mode<presentation>
\setbeamertemplate{navigation symbols}{}
\begin{document}
  \begin{frame}
    \centering
EOF
if [ -e Overlay_mKK_${suffix}.pdf ]
then
  echo "    \includegraphics[width=0.49\textwidth]{Overlay_mKK_${suffix}.pdf}" >> projections.tex
fi
if [ -e Overlay_mKK_${suffix}.pdf.~2~ ]
then
mv Overlay_mKK_${suffix}.pdf.~2~ Overlay_mKK_${suffix}.pdf
fi
if [ -e Overlay_mKK_${suffix}.pdf.~1~ ]
then
mv -v Overlay_mKK_${suffix}.pdf Overlay_mKK_high_${suffix}.pdf
mv -v Overlay_mKK_${suffix}.pdf.~1~ Overlay_mKK_${suffix}.pdf
cat <<EOF >> projections.tex
    \includegraphics[width=0.49\textwidth]{Overlay_mKK_high_${suffix}.pdf}\\\\\vfill
    \includegraphics[width=0.33\textwidth]{Overlay_phi_${suffix}.pdf}
    \includegraphics[width=0.33\textwidth]{Overlay_ctheta_1_${suffix}.pdf}
    \includegraphics[width=0.33\textwidth]{Overlay_ctheta_2_${suffix}.pdf}
EOF
else
cat <<EOF >> projections.tex
    \includegraphics[width=0.49\textwidth]{Overlay_phi_${suffix}.pdf}\\\\
    \includegraphics[width=0.49\textwidth]{Overlay_ctheta_1_${suffix}.pdf}
    \includegraphics[width=0.49\textwidth]{Overlay_ctheta_2_${suffix}.pdf}
EOF
fi
cat <<EOF >> projections.tex
  \end{frame}
\end{document}
EOF
pdflatex -interaction=batchmode projections.tex
cd ..
mv -v tmp/projections.tex $recentdir
mv -v tmp/projections.pdf $recentdir
rm -rf tmp
