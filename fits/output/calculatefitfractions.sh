#!/bin/bash
recentxml=$(find . -type f -name "outputXMLFile*" | sort | tail -1)
recentdir=$(find . -type d -name "RapidFitOutput_*" | sort | tail -1)
if [ "$recentxml" == "" ]
then
  echo "Can't find a outputXMLFile."
  exit 0
fi
tmpxml="Temp_Fixed.xml"
sed "s/[Ff]loat/Fixed/g" $recentxml | sed "s/[Ff]ree/Fixed/g" | sed "s/UseAcceptance:True/UseAcceptance:False/g" | sed "s/convolve:[Tt]rue/convolve:False/g" > $tmpxml
fitting -f $tmpxml --calculateFitFractions --OverrideXML /RapidFit/ToFit/DataSet/NumberEvents 1 --OverrideXML /RapidFit/ParameterSet/PhysicsParameter/signal_fraction/Value 1.0 --OverrideXML /RapidFit/ToFit/DataSet/PhaseSpaceBoundary/Observable/mKK/Maximum 1.80
rm $tmpxml
xmltimestamp=$(echo $recentxml | sed -r 's/outputXMLFile(.*)\.xml/\1/')
dirtimestamp=$(echo $recentdir | sed 's/RapidFitOutput_//')
if [ "$recentdir" == "" ]
then
  echo "Can't find a RapidFitOutput dir. Fit fractions will be calculated, but the file fitFractions.root is in danger of being overwritten later."
else
  mv -v fitFractions.root $recentdir
fi
