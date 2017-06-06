#!/bin/bash
source mvaopticut.sh
modes=(BsphiKK_MC BsphiKK_data)
for mode in ${modes[@]}
do
../bin/PlotEffvsmKK ../ntuples/${mode}_mva.root ../latex/figs/${mode}_mlp "mlp>${optcut}"
done

