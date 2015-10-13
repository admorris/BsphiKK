#!/bin/bash
source eos.sh
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/GetTrigEff ~/${EOS_nTuples_dir}/${mode}_nocut.root | tee log/${mode}_trigeff.log
done
# Unmount EOS after use
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
###########################################################
exit 0
