#!/bin/bash
cd ../fits
fitting -f acceptance.xml --calculateAcceptanceCoefficients
mv LegendreMoments.root LegendreMoments_Acceptance.root
mv sampled_LegendreMomentShape.root sampled_acceptance.root
../bin/PlotAngAcc sampled_acceptance.root
mv -v acceptance_*pdf ../latex/figs/
exit 0

