#!/bin/bash
source eos.sh
cd ../fits
fitting -f acceptance.xml --calculateAcceptanceCoefficients
rename LegendreMoments LegendreMoments_Acceptance LegendreMoments_*.root
cp -v LegendreMoments_Acceptance_*.root ${nTuples_dir}
rename LegendreMomentShape acceptance sampled_LegendreMomentShape_*.root
for file in $(ls sampled_acceptance_*.root)
do
  ../bin/PlotAngAcc $file $(echo $file | sed 's/sampled_acceptance_//g' | sed 's/\.root//g')
done
mv -v acceptance_*pdf ../latex/figs/
exit 0

