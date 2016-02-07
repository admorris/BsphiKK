#!/bin/bash
cd ../fits
fitting -f acceptance.xml --calculateAcceptanceCoefficients | tee acceptance.log
mv LegendreMoments.root LegendreMoments_Acceptance.root
../bin/PlotAngAcc sampled_acceptance.root acceptance.root
mv acceptance_*pdf ../latex/figs/

