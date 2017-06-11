#!/bin/bash
cd ../
for i in `seq 1 8`
do
	iminusone=$(echo "$i - 1" | bc -l)
	bin/GetResolution -F ntuples/BsphiKK_MC_mvacut.root -P Kplus0 Kminus0 -B BCON_KK_M -l -10 -u 10 -O latex/figs/mKKresolution_${i} -C "BCON_KK_M>1${iminusone}00&&BCON_KK_M<1${i}00&&KK_TRUEID!=333" 2>&1| tee scripts/log/KKresolution_${i}.log
done

