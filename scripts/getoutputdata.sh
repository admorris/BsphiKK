#! /bin/bash
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r3p1/InstallArea/scripts/LbLogin.sh
source ~/.bash_profile
source ~/.bashrc
source /etc/bashrc
SetupProject.sh LHCbDirac
lhcb-proxy-init
ntuple_name='BsphiKK'
user=admorris
###########################################################
user_fl=${user:0:1}
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r3p1/InstallArea/scripts/SetupProject.sh LHCbDirac
source /afs/cern.ch/project/eos/installation/lhcb/etc/setup.sh
if [ ! -d ~/${EOS_HOME}/ ]
then
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse mount ~/eos
fi
for(( jobno=481; jobno<=484; jobno++)); do
#if [ "${jobno}" -gt "424" -a "${jobno}" -lt "429" ]
if [ "${jobno}" == "482" ]
then
continue
fi
# Copy outputs to tmp
gangaout=~/gangadir/workspace/${user}/LocalXML/
lfns='LFNs.txt'
cd /tmp/${user}
mkdir ${jobno}tmp
cd /tmp/${user}/${jobno}tmp
find ${gangaout}${jobno} -name "${lfns}" -exec cat {} \; >output_lfns.lst
n=0
while read line
do
if [[ $line == *"${ntuple_name}.root"* ]]
then
echo "Retrieving LFN $n: " $line
dirac-dms-get-file $line
mv ${ntuple_name}.root ${ntuple_name}${n}_noPID.root
cutapplier ${ntuple_name}${n}_noPID.root DecayTreeTuple/DecayTree "Kminus_ProbNNk*(1-Kminus_ProbNNpi)>0.01&&Kplus_ProbNNk*(1-Kplus_ProbNNpi)>0.01&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.01&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.01" ${ntuple_name}${n}_PIDcut.root
n=$(($n+1))
fi
done < output_lfns.lst
hadd -f ../${ntuple_name}_${jobno}_PIDcut.root ${ntuple_name}*_PIDcut.root
hadd -f ../${ntuple_name}_${jobno}_noPID.root ${ntuple_name}*_noPID.root
if [ -e ../${ntuple_name}_${jobno}_PIDcut.root -a -e ../${ntuple_name}_${jobno}_noPID.root ]
then
rm ${ntuple_name}*root
fi
mv ../${ntuple_name}_${jobno}*.root ~/${EOS_HOME}/phiKK/
done
source /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select -b fuse umount ~/eos
