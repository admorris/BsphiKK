#!/bin/bash
cd ../mva
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r4p2/InstallArea/scripts/SetupProject.sh Gaudi ROOT -v 5.34
#Input variables (training sample)
root -l -b -q variables.C+\(\"TMVA.root\",\"InputVariables_Id\",\"TMVA\ Input\ Variables\"\)
#Input variable linear correlation coefficients
root -l -b -q correlations.C\(\"TMVA.root\"\)
#Classifier Output Distributions (test sample)
root -l -b -q mvas.C\(\"TMVA.root\",0\)
#Classifier Output Distributions (test and training samples superimposed)
root -l -b -q mvas.C\(\"TMVA.root\",3\)
#Classifier Background Rejection vs Signal Efficiency (ROC curve)
root -l -b -q efficiencies.C\(\"TMVA.root\"\)
#Classifier Cut Efficiencies
sigN=$(grep "initialmassfitABSignalNthreesigma" ../scripts/tables/MassFits.csv | sed -r 's/^[a-zA-Z]*\s+[a-zA-Z]*\s+([0-9\.]+).*/\1/')
bkgN=$(grep "initialmassfitABCombinatorialNthreesigma" ../scripts/tables/MassFits.csv | sed -r 's/^[a-zA-Z]*\s+[a-zA-Z]*\s+([0-9\.]+).*/\1/')
root -l -b <<EOF
.L ../lib/libResultDB.so
.L mvaeffs.C+
mvaeffs("TMVA.root",${sigN},${bkgN})
.q
EOF
cp -v plots/*.pdf ../latex/figs/
