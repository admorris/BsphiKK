#!/bin/bash
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
