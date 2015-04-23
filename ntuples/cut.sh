#! /bin/bash

source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r3p1/InstallArea/scripts/LbLogin.sh
ntuple_name='BsphiKK'
user=admorris
###########################################################
user_fl=${user:0:1}
EOS_nTuples_dir="/eos/lhcb/user/${user_fl}/${user}/phiKK/"
source /afs/cern.ch/project/eos/installation/lhcb/etc/setup.sh
if [ ! -d ~/${EOS_nTuples_dir}/ ]
then
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount ~/eos
fi

# The lines hereafter and before definition of cuts not normally necessary
cd ~/${EOS_nTuples_dir}/
pwd
hadd -f Bsphiphi_MC_nocut.root BsphiKK_468_PIDcut.root BsphiKK_469_PIDcut.root
hadd -f BsphiKK_MC_nocut.root BsphiKK_470_PIDcut.root BsphiKK_471_PIDcut.root
hadd -f BdphiKst_MC_nocut.root BsphiKK_472_PIDcut.root BsphiKK_473_PIDcut.root
hadd -f Bsphipipi_MC_nocut.root BsphiKK_474_PIDcut.root BsphiKK_475_PIDcut.root
hadd -f LbphiKp_MC_nocut.root BsphiKK_476_PIDcut.root BsphiKK_477_PIDcut.root
hadd -f BsphiKK_data_nocut.root BsphiKK_478_PIDcut.root BsphiKK_479_PIDcut.root BsphiKK_480_PIDcut.root BsphiKK_481_PIDcut.root
cd -
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
BsMcut="B_s0_MM>5100&&B_s0_MM<5900"
#Bs flight distance chi-squared cut
BsFDCHI2cut="B_s0_FDCHI2_OWNPV>250"
BsIPCHI2cut="B_s0_IPCHI2_OWNPV<20"
#PID cuts to get better Kaons
KPIDcut="Kminus_ProbNNk>0.1&&Kplus_ProbNNk>0.1&&Kminus0_ProbNNk>0.1&&Kplus0_ProbNNk>0.1"
KpiPIDcut="Kminus_ProbNNk*(1-Kminus_ProbNNpi)>0.025&&Kplus_ProbNNk*(1-Kplus_ProbNNpi)>0.025&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.025&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.025"
KpPIDcut="Kplus_ProbNNk*(1-Kplus_ProbNNp)>0.01&&Kminus_ProbNNk*(1-Kminus_ProbNNp)>0.01&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.01&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.01"
#Sum of all cuts
totalcut="${trigcut}&&${ghstcut}&&${trackisMuoncut}&&${phiMcut}&&${KpTcut}&&${BsMcut}&&${BsFDCHI2cut}&&${BsIPCHI2cut}&&${KPIDcut}&&${KpiPIDcut}&&${KpPIDcut}"

cutapplier ~/${EOS_nTuples_dir}/BsphiKK_data_nocut.root DecayTreeTuple/DecayTree "${totalcut}" BsphiKK_data_duplicates.root
cutapplier ~/${EOS_nTuples_dir}/BsphiKK_MC_nocut.root DecayTreeTuple/DecayTree "${totalcut}" BsphiKK_MC_duplicates.root
cutapplier ~/${EOS_nTuples_dir}/Bsphiphi_MC_nocut.root DecayTreeTuple/DecayTree "${totalcut}" Bsphiphi_MC_duplicates.root

root -q -b FlagClones.C+

cutapplier BsphiKK_data_duplicates_Clone.root DecayTree "isDup==1" BsphiKK_data_cuts.root
cutapplier BsphiKK_MC_duplicates_Clone.root DecayTree "isDup==1" BsphiKK_MC_cuts.root
cutapplier Bsphiphi_MC_duplicates_Clone.root DecayTree "isDup==1" Bsphiphi_MC_cuts.root

mv -v *duplicates* ~/${EOS_nTuples_dir}/
