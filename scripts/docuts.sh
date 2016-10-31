#!/bin/bash
source cuts.sh
source eos.sh
LbLogin.sh
source /afs/cern.ch/project/eos/installation/lhcb/etc/setup.sh
mkdir -p ~/eos
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount ~/eos
cd ../ntuples/
#Sum of all cuts
totalcut="${trigcut}&&${ghstcut}&&${trackisMuoncut}&&${phiMcut}&&${KpTcut}&&${phiIPCHI2cut}&&${KKIPCHI2cut}&&${BsFDCHI2cut}&&${BsIPCHI2cut}&&${KpiPIDcut}&&${KpPIDcut}"
###########################################################
modes=(BsphiKK_data BsphiKK_sideband Bsphiphi_MC BsphiKK_MC BdphiKst_MC Bsphipipi_MC LbphiKp_MC)
for mode in ${modes[@]}
do
  if [ "$mode" == "Bsphiphi_MC"  -o  "$mode" == "BsphiKK_MC" ]
  then
    applied_cut="${totalcut}&&${BKGCATcut}&&${BsMcut}"
  elif [ "$mode" == "BsphiKK_sideband" ]
  then
    applied_cut="${totalcut}"
  else
    applied_cut="${totalcut}&&${BsMcut}"
  fi
  cutapplier ~/${EOS_nTuples_dir}${mode}_loosePID.root DecayTreeTuple/DecayTree ${applied_cut} ${mode}_cuts.root &
done
wait
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
exit 0

