#!/bin/bash
source eos.sh
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/GetTrigEff ${nTuples_dir}${mode}_nocut.root --save --output-file ../scripts/tables/TrigEffs.csv | tee log/${mode}_trigeff.log
done
LbLogin.sh
source /afs/cern.ch/project/eos/installation/lhcb/etc/setup.sh
mkdir -p ~/eos
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount ~/eos
cutapplier ${EOS_nTuples_dir}BsphiKK_MC_nocut.root "DecayTreeTuple/DecayTree" "BCON_KK_M<1800" ../ntuples/BsphiKK_MC_mKKcut.root
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
../bin/GetTrigEff ../ntuples/BsphiKK_MC_mKKcut.root --save --output-file ../scripts/tables/TrigEffs.csv  | tee log/BsphiKK_MC_mKKcut_trigeff.log
rm -v ../ntuples/BsphiKK_MC_mKKcut.root
../bin/ExportResults ../scripts/tables/TrigEffs.csv ../latex/results/TrigEffs.tex
exit 0

