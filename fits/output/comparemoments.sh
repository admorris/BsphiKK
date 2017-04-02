#!/bin/bash
if [ "$1" == "" ]
then
  ntuple="root://eoslhcb.cern.ch//eos/lhcb/user/a/admorris/phiKK/ntuples/BsphiKK_data_mvacut.root"
else
  ntuple=$1
fi
recentxml=$(find . -type f -name "outputXMLFile*" | sort | tail -1)
recentdir=$(find . -type d -name "RapidFitOutput_*" | sort | tail -1)
if [ "$recentxml" == "" ]
then
  echo "Can't find a outputXMLFile. Please provide it as an argument."
  exit 0
fi
if [ "$recentdir" == "" ]
then
  echo "Can't find a RapidFitOutput dir. Please provide it as an argument."
  exit 0
fi
xmltimestamp=$(echo $recentxml | sed -r 's/outputXMLFile(.*)\.xml/\1/')
dirtimestamp=$(echo $recentdir | sed 's/RapidFitOutput_//')
if [ "$xmltimestamp" == "$dirtimestamp" ]
then
  fitting -f $recentxml --saveOneDataSet toys.root --OverrideXML /RapidFit/ToFit/DataSet/NumberEvents 1000000
  mv -v toys.root $recentdir
  cd $recentdir
  CompareMoments --MCfile toys.root --MCmass mKK --MCangle ctheta_2 --lower 1.05 --bins 25 --CDfile $ntuple --CDweight Nsig_sw --CDcut "B_s0_LOKI_Mass>5326.12&&5406.56>B_s0_LOKI_Mass"
else
  echo "Most recent XML timestamp ($xmltimestamp) does not match most recent output dir timestamp ($dirtimestamp). Will not generate toys."
fi
