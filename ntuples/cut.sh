#! /bin/bash
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
###########################################################
#Same trigger lines as Haofei's analysis
trigcut="(B_s0_L0HadronDecision_TOS||B_s0_L0Global_TIS)&&B_s0_Hlt1TrackAllL0Decision_TOS&&(B_s0_Hlt2Topo2BodyBBDTDecision_TOS||B_s0_Hlt2Topo3BodyBBDTDecision_TOS||B_s0_Hlt2Topo4BodyBBDTDecision_TOS||B_s0_Hlt2IncPhiDecision_TOS)"
#Tighter cut on track ghost prob than stripping
ghstcut="Kminus_TRACK_GhostProb<0.3&&Kplus_TRACK_GhostProb<0.3&&Kminus0_TRACK_GhostProb<0.3&&Kplus0_TRACK_GhostProb<0.3"
#Get rid of K->mu
trackisMuoncut="Kminus_isMuon==0&&Kplus_isMuon==0&&Kminus0_isMuon==0&&Kplus0_isMuon==0"
#Kaon PT>500 (as in PhiRhoLine) to all Kaons
KpTcut="Kminus_PT>500&&Kplus_PT>500&&Kminus0_PT>500&&Kplus0_PT>500"
#Phi mass window cut
phiMcut="TMath::Abs(phi_1020_MM-1019.461)<15"
#Bs mass window cut
BsMcut="B_s0_LOKI_Mass>5000&&B_s0_LOKI_Mass<6000"
#Bs flight distance chi-squared cut
BsFDCHI2cut="B_s0_FDCHI2_OWNPV>250"
BsIPCHI2cut="B_s0_IPCHI2_OWNPV<20"
#PID cuts to get better Kaons
KPIDcut="Kminus_ProbNNk>0.1&&Kplus_ProbNNk>0.1&&Kminus0_ProbNNk>0.1&&Kplus0_ProbNNk>0.1"
KpiPIDcut="Kminus_ProbNNk*(1-Kminus_ProbNNpi)>0.025&&Kplus_ProbNNk*(1-Kplus_ProbNNpi)>0.025&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.025&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.025"
KpPIDcut="Kplus_ProbNNk*(1-Kplus_ProbNNp)>0.01&&Kminus_ProbNNk*(1-Kminus_ProbNNp)>0.01&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.01&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.01"
#Monte Carlo background category
BKGCATcut="(B_s0_BKGCAT<20||B_s0_BKGCAT==50)"
#Sum of all cuts
totalcut="${trigcut}&&${ghstcut}&&${trackisMuoncut}&&${phiMcut}&&${KpTcut}&&${BsMcut}&&${BsFDCHI2cut}&&${BsIPCHI2cut}&&${KPIDcut}&&${KpiPIDcut}&&${KpPIDcut}"
###########################################################
modes=(Bsphiphi_MC BsphiKK_MC BdphiKst_MC Bsphipipi_MC LbphiKp_MC BsphiKK_data)
for mode in ${modes[@]}; do
if [ "$mode" == "Bsphiphi_MC"  -o  "$mode" == "BsphiKK_MC" ]
then
cutapplier ~/${EOS_nTuples_dir}/${mode}_nocut.root DecayTreeTuple/DecayTree "${totalcut}&&${BKGCATcut}" ${mode}_duplicates.root
else
cutapplier ~/${EOS_nTuples_dir}/${mode}_nocut.root DecayTreeTuple/DecayTree "${totalcut}" ${mode}_duplicates.root
fi
root -q -b -l "FlagClones.C+(\"${mode}_duplicates.root\")"
cutapplier ${mode}_duplicates_Clone.root DecayTree "isDup==1" ${mode}_cuts.root
done
###########################################################
# Move intermediate files to EOS
mv -v *duplicates*root ~/${EOS_nTuples_dir}/
# Unmount EOS after use
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
exit 0
