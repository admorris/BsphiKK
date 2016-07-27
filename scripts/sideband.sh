#! /bin/bash
source eos.sh
source cuts.sh
LbLogin.sh
source /afs/cern.ch/project/eos/installation/lhcb/etc/setup.sh
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount ~/eos
cd ~/${EOS_nTuples_dir}/
cutapplier BsphiKK_data_nocut.root DecayTreeTuple/DecayTree ${sidebandcut} BsphiKK_sideband_nocut.root
cutapplier BsphiKK_data_loosePID.root DecayTreeTuple/DecayTree ${sidebandcut} BsphiKK_sideband_loosePID.root
cd -
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
exit 0
