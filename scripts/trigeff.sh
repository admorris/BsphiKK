#!/bin/bash
source eos.sh
modes=(BsphiKK_MC Bsphiphi_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/GetTrigEff ${EOS_server}${EOS_nTuples_dir}${mode}_nocut.root --save --output-file ../scripts/tables/TrigEffs.csv | tee log/${mode}_trigeff.log
done
cutapplier ${EOS_server}${EOS_nTuples_dir}BsphiKK_MC_nocut.root "DecayTreeTuple/DecayTree" "KK_M<1800" ../ntuples/BsphiKK_MC_mKKcut.root
../bin/GetTrigEff ../ntuples/BsphiKK_MC_mKKcut.root --save --output-file ../scripts/tables/TrigEffs.csv  | tee log/BsphiKK_MC_mKKcut_trigeff.log
rm ../ntuples/BsphiKK_MC_mKKcut.root
../bin/ExportResults ../scripts/tables/TrigEffs.csv ../latex/results/TrigEffs.tex
exit 0

