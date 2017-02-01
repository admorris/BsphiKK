#! /bin/bash
source eos.sh
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r6p9/InstallArea/scripts/LbLogin.sh
lhcb-proxy-init
ntuple_name='BsphiKK'
user=admorris
gangaout=~/gangadir/workspace/${user}/LocalXML/
lfns='LFNs.txt'
###########################################################
user_fl=${user:0:1}
for(( jobno=0; jobno<=1; jobno++))
do
  # Copy outputs to tmp
  cd /tmp/${user}
  mkdir ${jobno}tmp
  cd /tmp/${user}/${jobno}tmp
  echo ${gangaout}${jobno}/${lfns}
  cat ${gangaout}${jobno}/${lfns} >output_lfns.lst
  n=0
  while read line
  do
    echo "Retrieving LFN $n: " $line
    /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r6p9/InstallArea/scripts/lb-run LHCbDirac dirac-dms-get-file $line
    mv -v ${ntuple_name}.root ${ntuple_name}${n}_noPID.root
    cutapplier ${ntuple_name}${n}_noPID.root DecayTreeTuple/DecayTree "Kminus_ProbNNk*(1-Kminus_ProbNNpi)>0.01&&Kplus_ProbNNk*(1-Kplus_ProbNNpi)>0.01&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.01&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.01" ${ntuple_name}${n}_PIDcut.root
    n=$(($n+1))
  done < output_lfns.lst
  hadd -f ../${ntuple_name}_${jobno}_PIDcut.root ${ntuple_name}*_PIDcut.root
  hadd -f ../${ntuple_name}_${jobno}_noPID.root ${ntuple_name}*_noPID.root
  if [ -e ../${ntuple_name}_${jobno}_PIDcut.root -a -e ../${ntuple_name}_${jobno}_noPID.root ]
  then
    rm -v ${ntuple_name}*root
  fi
  mv -v ../${ntuple_name}_${jobno}*.root ${EOS_nTuples_dir}
done
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
exit 0

