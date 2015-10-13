#!/bin/bash
source eos.sh
function effs
{
  L0=(B_s0_L0HadronDecision_TOS B_s0_L0Global_TIS)
  Hlt1=B_s0_Hlt1TrackAllL0Decision_TOS
  Hlt2=(B_s0_Hlt2Topo2BodyBBDTDecision_TOS B_s0_Hlt2Topo3BodyBBDTDecision_TOS B_s0_Hlt2Topo4BodyBBDTDecision_TOS B_s0_Hlt2IncPhiDecision_TOS)
  levels=()
  if [ "$#" -ne "1" ]
  then
  echo "$# arguments provided, expected 1"
  echo "Usage: effs <filename>"
  exit 1
  fi
  for trig in ${L0[@]}
  do
  ../bin/CutEff $1 "B_s0_M" "B_s0_M>0" "$trig"
  done
  ../bin/CutEff $1 "B_s0_M" "${L0[0]}||${L0[1]}" "$Hlt1"
  for trig in ${Hlt2[@]}
  do
  ../bin/CutEff $1 "B_s0_M" "(${L0[0]}||${L0[1]})&&$Hlt1" "$trig"
  done
  ../bin/CutEff $1 "B_s0_M" "(${L0[0]}||${L0[1]})&&$Hlt1" "(${Hlt2[0]}||${Hlt2[1]}||${Hlt2[2]}||${Hlt2[3]})"
  
}
export effs
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_data)
for mode in ${modes[@]}
do
effs ~/${EOS_nTuples_dir}/${mode}_nocut.root | tee log/${mode}_trigeff.log
done
# Unmount EOS after use
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
###########################################################
exit 0
