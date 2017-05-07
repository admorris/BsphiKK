#!/bin/bash
source cuts.sh
source eos.sh
cd ../ntuples/
#Sum of all cuts
totalcut="${trigcut}&&${ghstcut}&&${trackisMuoncut}&&${phiMcut}&&${KpTcut}&&${StdTightKaons}&&${phiIPCHI2cut}&&${KKIPCHI2cut}&&${KKEVCHI2cut}&&${BsFDCHI2cut}&&${BsIPCHI2cut}&&${KpiPIDcut}&&${KpPIDcut}"
###########################################################
modes=(BsphiKK_data BsphiKK_sideband Bsphiphi_MC BsphiKK_MC BdphiKst_MC Bsphipipi_MC LbphiKp_MC)
for mode in ${modes[@]}
do
  if [ "$mode" == "Bsphiphi_MC"  -o  "$mode" == "BsphiKK_MC" ]
  then
    applied_cut="${totalcut}&&${signalBKGCATcut}"
  elif [[ "$mode" == *"_MC" ]]
  then
    applied_cut="${totalcut}&&${misIDBKGCATcut}"
  else
    applied_cut="${totalcut}"
  fi
  cutapplier ${nTuples_dir}${mode}_loosePID.root DecayTreeTuple/DecayTree ${applied_cut} ${mode}_cuts.root &
done
wait


