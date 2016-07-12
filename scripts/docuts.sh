#!/bin/bash
source cuts.sh
source eos.sh
cd ../ntuples/
#Sum of all cuts
totalcut="${trigcut}&&${ghstcut}&&${trackisMuoncut}&&${phiMcut}&&${KpTcut}&&${phiIPCHI2cut}&&${KKIPCHI2cut}&&${BsFDCHI2cut}&&${BsIPCHI2cut}&&${KpiPIDcut}&&${KpPIDcut}"
###########################################################
modes=(BsphiKK_data BsphiKK_sideband Bsphiphi_MC BsphiKK_MC BdphiKst_MC Bsphipipi_MC LbphiKp_MC)
for mode in ${modes[@]}
do
  if [ "$mode" == "Bsphiphi_MC"  -o  "$mode" == "BsphiKK_MC" ]
  then
    cutapplier ${EOS_server}${EOS_nTuples_dir}${mode}_nocut.root DecayTreeTuple/DecayTree "${totalcut}&&${BKGCATcut}&&${BsMcut}" ${mode}_duplicates.root &
  elif [ "$mode" == "BsphiKK_sideband" ]
  then
    cutapplier ${EOS_server}${EOS_nTuples_dir}BsphiKK_data_nocut.root DecayTreeTuple/DecayTree "${totalcut}&&B_s0_LOKI_Mass>5500" ${mode}_duplicates.root &
  else
    cutapplier ${EOS_server}${EOS_nTuples_dir}${mode}_nocut.root DecayTreeTuple/DecayTree "${totalcut}&&${BsMcut}" ${mode}_duplicates.root &
  fi
done
wait
for mode in ${modes[@]}
do
  ../bin/FlagClones ${mode}_duplicates.root DecayTreeTuple/DecayTree
  cutapplier ${mode}_duplicates_Clone.root DecayTree "isDup==1" ${mode}_cuts.root
done
###########################################################
# Move intermediate files to EOS
LbLogin.sh
source /afs/cern.ch/project/eos/installation/lhcb/etc/setup.sh
/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp *duplicates*root ${EOS_nTuples_dir}
rm -v *duplicates*root
###########################################################
exit 0

