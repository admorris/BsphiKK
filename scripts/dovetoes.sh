#!/bin/bash
source cuts.sh
cd ../ntuples
echo $phiwindow
################################################################################
for mode in $(ls *_mvaVars.root | sed 's/_mvaVars\.root//'); do
cutapplier ${mode}_mvaVars.root DecayTree "${phikstveto}&&${LbphiKpveto}&&${Lcphipveto}&&${DtoKaonsveto}&&${Dtophipiveto}" ${mode}_mvaVars_vetoes.root
done
exit 0
