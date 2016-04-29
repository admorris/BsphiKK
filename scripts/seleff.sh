#!/bin/bash
source eos.sh
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/GetSelEff ~/${EOS_nTuples_dir}/${mode}_nocut.root --save --output-file ../scripts/tables/SelEffs.csv  | tee log/${mode}_seleff.log
done
# Unmount EOS after use
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
../bin/ExportResults ../scripts/tables/SelEffs.csv ../latex/SelEffs.tex
###########################################################
exit 0
