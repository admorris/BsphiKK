#! /bin/bash
source eos.sh
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount ~/eos
cd ~/${EOS_nTuples_dir}/
pwd
hadd -f Bsphiphi_MC_nocut.root BsphiKK_468_PIDcut.root BsphiKK_469_PIDcut.root &
hadd -f BsphiKK_MC_nocut.root BsphiKK_470_PIDcut.root BsphiKK_471_PIDcut.root &
hadd -f BdphiKst_MC_nocut.root BsphiKK_472_PIDcut.root BsphiKK_473_PIDcut.root &
hadd -f Bsphipipi_MC_nocut.root BsphiKK_474_PIDcut.root BsphiKK_475_PIDcut.root &
hadd -f LbphiKp_MC_nocut.root BsphiKK_476_PIDcut.root BsphiKK_477_PIDcut.root &
hadd -f BsphiKK_data_nocut.root BsphiKK_478_PIDcut.root BsphiKK_479_PIDcut.root BsphiKK_480_PIDcut.root BsphiKK_481_PIDcut.root BsphiKK_483_PIDcut.root BsphiKK_484_PIDcut.root &
wait
cd -
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
exit 0
