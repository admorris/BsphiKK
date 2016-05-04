#!/bin/bash
source cuts.sh
cd ../ntuples
################################################################################
for mode in $(ls *data*_mvaVars.root *MC*_mvaVars.root | sed 's/_mvaVars\.root//'); do
cutapplier ${mode}_mvaVars.root DecayTree "${phikstveto}&&${LbphiKpveto}&&${Lcphipveto}&&${DtoKaonsveto}&&${Dtophipiveto}" ${mode}_mvaVars_vetoes.root &
done
wait
exit 0
