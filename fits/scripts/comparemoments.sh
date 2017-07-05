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
  fitting -f $recentxml --saveOneDataSet highmasstoys.root --OverrideXML /RapidFit/ToFit/DataSet/NumberEvents 100000 --OverrideXML  /RapidFit/ToFit/DataSet/PhaseSpaceBoundary/Observable/mKK/Minimum 1.05 > $recentdir/toygeneration.log && \
  fitting -f $recentxml --saveOneDataSet phirangetoys.root --OverrideXML /RapidFit/ToFit/DataSet/NumberEvents 100000 --OverrideXML  /RapidFit/ToFit/DataSet/PhaseSpaceBoundary/Observable/mKK/Maximum 1.05 > $recentdir/toygeneration.log && \
  mv *toys.root $recentdir && cd $recentdir && \
  CompareMoments --MCfile highmasstoys.root --MCmass mKK --MCangle ctheta_2 --lower 1.05 --upper 1.80 --bins 25 --CDfile $ntuple --CDcut "B_s0_LOKI_Mass>5330&&5405>B_s0_LOKI_Mass" --plot LegendreMoments_highmass && \
  CompareMoments --MCfile phirangetoys.root --MCmass mKK --MCangle ctheta_2 --lower 0.99 --upper 1.05 --bins 30 --CDfile $ntuple --CDcut "B_s0_LOKI_Mass>5330&&5405>B_s0_LOKI_Mass" --plot LegendreMoments_phirange
else
  echo "Most recent XML timestamp ($xmltimestamp) does not match most recent output dir timestamp ($dirtimestamp). Will not generate toys."
fi
