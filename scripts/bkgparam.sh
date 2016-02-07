#!/bin/bash
cd ../fits
fitting -f background.xml --calculateBackgroundCoefficients | tee background.log
mv LegendreMoments.root LegendreMoments_Background.root
../bin/PlotAngAcc sampled_background.root background.root
mv background_*pdf ../latex/figs/

