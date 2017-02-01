#! /bin/bash
source eos.sh
cd ${EOS_nTuples_dir}/
pwd
hadd -f Bsphiphi_MC_loosePID.root BsphiKK_468_PIDcut.root BsphiKK_469_PIDcut.root &
hadd -f BsphiKK_MC_loosePID.root BsphiKK_573_PIDcut.root BsphiKK_574_PIDcut.root BsphiKK_576_PIDcut.root BsphiKK_577_PIDcut.root BsphiKK_0_PIDcut.root BsphiKK_1_PIDcut.root &
hadd -f BdphiKst_MC_loosePID.root BsphiKK_472_PIDcut.root BsphiKK_473_PIDcut.root &
hadd -f Bsphipipi_MC_loosePID.root BsphiKK_474_PIDcut.root BsphiKK_475_PIDcut.root &
hadd -f LbphiKp_MC_loosePID.root BsphiKK_476_PIDcut.root BsphiKK_477_PIDcut.root &
hadd -f BsphiKK_data_loosePID.root BsphiKK_478_PIDcut.root BsphiKK_479_PIDcut.root BsphiKK_480_PIDcut.root BsphiKK_481_PIDcut.root BsphiKK_483_PIDcut.root BsphiKK_484_PIDcut.root &
hadd -f Bsphiphi_MC_nocut.root BsphiKK_468_noPID.root BsphiKK_469_noPID.root &
hadd -f BsphiKK_MC_nocut.root BsphiKK_573_noPID.root BsphiKK_574_noPID.root BsphiKK_576_noPID.root BsphiKK_577_noPID.root BsphiKK_0_noPID.root BsphiKK_1_noPID.root &
hadd -f BdphiKst_MC_nocut.root BsphiKK_472_noPID.root BsphiKK_473_noPID.root &
hadd -f Bsphipipi_MC_nocut.root BsphiKK_474_noPID.root BsphiKK_475_noPID.root &
hadd -f LbphiKp_MC_nocut.root BsphiKK_476_noPID.root BsphiKK_477_noPID.root &
hadd -f BsphiKK_data_nocut.root BsphiKK_478_noPID.root BsphiKK_479_noPID.root BsphiKK_480_noPID.root BsphiKK_481_noPID.root BsphiKK_483_noPID.root BsphiKK_484_noPID.root &
wait
cd -
exit 0

