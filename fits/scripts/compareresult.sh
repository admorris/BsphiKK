#!/bin/bash
function parse()
{
  grep "$1" $recentdir/SimpleTable.tex | sed 's/^[^&]*&\s*//' | sed 's/\s*&.*\\\\//' | sed 's/\\pm/,/' | sed 's/\s//g'
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
APplus=$(parse "phi1020\\\_Aplussq")
APzero=$(parse "phi1020\\\_Azerosq")
deltaPplus=$(parse "phi1020\\\_deltaplus")
deltaPminus=$(parse "phi1020\\\_deltaminus")
reldir=$(echo $0 | sed s'/\/[^\/]*sh//')
root -q -b -l "$reldir/compareresult.C({$APplus},{$deltaPplus},{$APzero},{$deltaPminus})"
mv Aperpsq.pdf Azerosq.pdf deltapara.pdf $recentdir

