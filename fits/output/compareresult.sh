#!/bin/bash
function parse()
{
  grep "$1" $recentdir/MinimalTable.tex | sed 's/^.*&\s*//' | sed 's/\\\\//' | sed 's/\\pm/,/' | sed 's/\s//g'
}
if [ "$1" == "" ]
then
  recentdir=$(find . -type d -name "RapidFitOutput_*" | sort | tail -1)
else
  recentdir=$1
fi
if [ "$recentdir" == "" ]
then
  echo "Can't find a RapidFitOutput dir. Please provide it as an argument."
  exit 0
fi
APperp=$(parse "APperp")
APzero=$(parse "APzero")
deltaPpara=$(parse "deltaPpara")
reldir=$(echo $0 | sed s'/\/[^\/]*sh//')
root -q -b -l "$reldir/compareresult.C({$APperp},{$APzero},{$deltaPpara})"
mv Aperpsq.pdf Azerosq.pdf deltapara.pdf $recentdir

