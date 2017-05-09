#!/bin/bash
cd ../mva
#source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r4p2/InstallArea/scripts/SetupProject.sh Gaudi ROOT -v 5.34
#Input variables (training sample)
root -l -b -q variables.C+\(\"TMVA.root\",\"InputVariables_Id\",\"TMVA\ Input\ Variables\"\)
#Input variable linear correlation coefficients
root -l -b -q correlations.C\(\"TMVA.root\"\)
#Classifier Output Distributions (test sample)
root -l -b -q mvas.C+\(\"TMVA.root\",MVAType\)
#Classifier Output Distributions (test and training samples superimposed)
root -l -b -q mvas.C+\(\"TMVA.root\",CompareType\)
#Classifier Background Rejection vs Signal Efficiency (ROC curve)
root -l -b -q efficiencies.C\(\"TMVA.root\"\)


