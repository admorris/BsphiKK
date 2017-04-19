#!/bin/bash
recentxml=$(find . -type f -name "outputXMLFile*" | sort | tail -1)
recentdir=$(find . -type d -name "RapidFitOutput_*" | sort | tail -1)
if [ "$recentxml" == "" ]
then
  echo "Can't find a outputXMLFile."
  exit 0
fi
if [ "$recentdir" == "" ]
then
  echo "Can't find a RapidFitOutput dir. Fit fractions will be calculated, but the file fitFractions.root is in danger of being overwritten later."
fi
tmpxml="Temp_Fixed.xml"
sed "s/[Ff]loat/Fixed/g" $recentxml | sed "s/[Ff]ree/Fixed/g" | sed "s/UseAcceptance:True/UseAcceptance:False/g" > $tmpxml
fitting -f $tmpxml --calculateFitFractions --OverrideXML /RapidFit/ToFit/DataSet/NumberEvents 1
rm $tmpxml
xmltimestamp=$(echo $recentxml | sed -r 's/outputXMLFile(.*)\.xml/\1/')
dirtimestamp=$(echo $recentdir | sed 's/RapidFitOutput_//')
if [ "$xmltimestamp" == "$dirtimestamp" ]
then
  mv -v fitFractions.root $recentdir
else
  echo "Most recent XML timestamp ($xmltimestamp) does not match most recent output dir timestamp ($dirtimestamp). I don't know where to put the file fitFractions.root"
fi
