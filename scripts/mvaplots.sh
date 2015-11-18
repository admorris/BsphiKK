#!/bin/bash
cd ../mva
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r4p2/InstallArea/scripts/SetupProject.sh Gaudi ROOT -v 5.34
#Input variables (training sample)
root -l -q variables.C+\(\"TMVA.root\",\"InputVariables_Id\",\"TMVA Input Variables\"\)
#Input variable linear correlation coefficients
root -l -q correlations.C\(\"TMVA.root\"\)
#Classifier Output Distributions (test sample)
root -l -q mvas.C\(\"TMVA.root\",0\)
#Classifier Output Distributions (test and training samples superimposed)
root -l -q mvas.C\(\"TMVA.root\",3\)
#Classifier Background Rejection vs Signal Efficiency (ROC curve)
root -l -q efficiencies.C\(\"TMVA.root\"\)
#Classifier Cut Efficiencies
root -l -q mvaeffs.C+\(\"TMVA.root\"\)
cp -v plots/*.pdf ../latex/figs/
