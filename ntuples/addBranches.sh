#! /bin/bash
source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r3p1/InstallArea/scripts/LbLogin.sh
###########################################################
for mode in $(ls *_cuts.root | sed 's/_cuts\.root//'); do
root -q -b -l "addBranches.C+(\"${mode}\")"
done
exit 0
