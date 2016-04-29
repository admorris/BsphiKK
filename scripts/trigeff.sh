#!/bin/bash
source eos.sh
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/GetTrigEff ~/${EOS_nTuples_dir}/${mode}_nocut.root --save --output-file ../scripts/tables/TrigEffs.csv | tee log/${mode}_trigeff.log
done
cutapplier ~/${EOS_nTuples_dir}/BsphiKK_MC_nocut.root "DecayTreeTuple/DecayTree" "KK_M<1800" ../ntuples/BsphiKK_MC_1800_nocut.root
../bin/GetTrigEff ../ntuples/BsphiKK_MC_1800_nocut.root --save --output-file ../scripts/tables/TrigEffs.csv  | tee log/BsphiKK_MC_1800_trigeff.log
rm ../ntuples/BsphiKK_MC_1800_nocut.root
# Unmount EOS after use
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
../bin/ExportResults ../scripts/tables/TrigEffs.csv ../latex/TrigEffs.tex
###########################################################
exit 0
