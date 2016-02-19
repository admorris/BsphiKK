#!/bin/bash
cd ../fits
fitting -f acceptance.xml --calculateAcceptanceCoefficients
mv LegendreMoments.root LegendreMoments_Acceptance.root
../bin/PlotAngAcc sampled_acceptance.root acceptance.root
mv acceptance_*pdf ../latex/figs/

