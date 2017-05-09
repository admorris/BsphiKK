#!/bin/bash
rm -f tables/mvaeffs.csv
cd ../mva
#source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r4p2/InstallArea/scripts/SetupProject.sh Gaudi ROOT -v 5.34
sigN=$(grep "initialmassfitABSignalNthreesigma" ../scripts/tables/MassFits.csv | sed -r 's/^[a-zA-Z]*\s+[a-zA-Z]*\s+([0-9\.]+).*/\1/')
bkgN=$(grep "initialmassfitABCombinatorialNthreesigma" ../scripts/tables/MassFits.csv | sed -r 's/^[a-zA-Z]*\s+[a-zA-Z]*\s+([0-9\.]+).*/\1/')
root -l -b <<EOF # wow
.L ../common/lib/libResultDB.so
.x mvaeffs.C+("TMVA.root",${sigN},${bkgN})
.q # I'm so sorry 
EOF
cp -v plots/*.pdf ../latex/figs/
# Store optimal cut
optcut=$(../bin/PrintResult ../scripts/tables/mvaeffs.csv MLPoptcut | grep "^value.*$" | sed -r 's/^value\s*:\s*([0-9\.]*.*$)/\1/')
echo "#!/bin/bash" > ../scripts/mvaopticut.sh
echo "optcut=$optcut" >> ../scripts/mvaopticut.sh
echo "" >> ../scripts/mvaopticut.sh

