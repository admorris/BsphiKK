#! /bin/bash
cd ../ntuples/
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r3p1/InstallArea/scripts/LbLogin.sh
ntuple_name='BsphiKK'
user=admorris
###########################################################
user_fl=${user:0:1}
# Setup EOS directory
EOS_nTuples_dir="/eos/lhcb/user/${user_fl}/${user}/phiKK/"
source /afs/cern.ch/project/eos/installation/lhcb/etc/setup.sh
if [ ! -d ~/${EOS_nTuples_dir}/ ]
then
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount ~/eos
fi
source cuts.sh
#Sum of all cuts
totalcut="${trigcut}&&${ghstcut}&&${trackisMuoncut}&&${phiMcut}&&${KpTcut}&&${BsFDCHI2cut}&&${BsIPCHI2cut}&&${KpiPIDcut}&&${KpPIDcut}"
###########################################################
modes=(BsphiKK_data BsphiKK_sideband Bsphiphi_MC BsphiKK_MC BdphiKst_MC Bsphipipi_MC LbphiKp_MC)
for mode in ${modes[@]}; do
if [ "$mode" == "Bsphiphi_MC"  -o  "$mode" == "BsphiKK_MC" ]
then
cutapplier ~/${EOS_nTuples_dir}/${mode}_nocut.root DecayTreeTuple/DecayTree "${totalcut}&&${BKGCATcut}&&${BsMcut}" ${mode}_duplicates.root
elif [ "$mode" == "BsphiKK_sideband" ]
then
cutapplier ~/${EOS_nTuples_dir}/BsphiKK_data_nocut.root DecayTreeTuple/DecayTree "${totalcut}&&B_s0_LOKI_Mass>5600" ${mode}_duplicates.root
else
cutapplier ~/${EOS_nTuples_dir}/${mode}_nocut.root DecayTreeTuple/DecayTree "${totalcut}&&${BsMcut}" ${mode}_duplicates.root
fi
../bin/FlagClones ${mode}_duplicates.root DecayTreeTuple/DecayTree
cutapplier ${mode}_duplicates_Clone.root DecayTree "isDup==1" ${mode}_cuts.root
done
###########################################################
# Move intermediate files to EOS
mv -v *duplicates*root ~/${EOS_nTuples_dir}/
# Unmount EOS after use
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
###########################################################

exit 0
