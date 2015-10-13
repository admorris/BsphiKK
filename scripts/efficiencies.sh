#!/bin/bash
source eos.sh
source cuts.sh
function effs
{
  if [ "$#" -ne "1" ]
  then
  echo "$# arguments provided, expected 1"
  echo "Usage: effs <filename>"
  exit 1
  fi
  for cut in ${cuts[@]}
  do
  if [ "$trigcut" != "$cut" ]
  then
  ../bin/CutEff $1 "B_s0_M" "$trigcut" "$cut"
  fi
  done
}
export effs
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_data)
for mode in ${modes[@]}
do
effs ~/${EOS_nTuples_dir}/${mode}_nocut.root | tee log/${mode}_efficiencies.log
done
# Unmount EOS after use
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
###########################################################
exit 0
