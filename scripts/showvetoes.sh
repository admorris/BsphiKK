suffices=(_mvaVars _mvaVars_vetoes)
branches=(phiKplusM phiKminusM phipipiM pipiM phipiplusM phipiminusM phiKpiM KpiM phipM phipbarM phiKpM KpM)
titles=("#it{#phi K}^{#plus}" "#it{#phi K}^{#minus}" "#it{#phi}#it{#pi}^{#plus}#it{#pi}^{#minus}" "#it{#pi}^{#plus}#it{#pi}^{#minus}" "#it{#phi}#it{#pi}^{#plus}" "#it{#phi}#it{#pi}^{#minus}" "#it{#phi K #pi}" "#it{K #pi}" "#it{#phi}#it{p}" "#it{#phi}#it{#bar{p}}" "#it{#phi K p}" "#it{K p}")
lower=(1500 1500 4800 250 1500 1500 5000 600 1500 1500 5500 1400)
upper=(3000 3000 5500 4000 3000 3000 5600 4000 3000 3000 6000 4500)
for i in `seq 0 11`
do
  for suffix in ${suffices[@]}
  do
    extra=""
    if [ "${branches[$i]}" == "phiKpM" ]
    then
      extra="--overlay ../ntuples/LbphiKp_MC${suffix}.root --scale 0.25"
    elif [ "${branches[$i]}" == "phiKpiM" ]
    then
      extra="--overlay ../ntuples/BdphiKst_MC${suffix}.root --scale 0.25"
    fi
    ../bin/AnnotateBranch \
      -F ../ntuples/BsphiKK_data${suffix}.root \
      -B ${branches[$i]} \
      -T "#it{m}(${titles[$i]})" \
      -O ../latex/figs/${branches[$i]}$suffix \
      -l ${lower[$i]} \
      -u ${upper[$i]} \
      -b 50 ${extra} &
  done
done
