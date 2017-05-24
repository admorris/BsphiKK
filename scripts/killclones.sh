#!/bin/bash
cd ../ntuples
function killclones()
{
  ../bin/FlagClones $1.root
  cutapplier $1_Clone.root DecayTree "isDup==1" $1_final.root
  rm -v $1_Clone.root
}
for file in $(ls *mvacut.root | sed 's/\.root//g')
do
  killclones ${file} > killclones_$file.log &
done
wait
for file in $(ls killclones_*.log)
{
  cat $file
  rm $file
}

