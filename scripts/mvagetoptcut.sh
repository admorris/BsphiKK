#!/bin/bash
rm -f tables/mvaeffs.csv
cd ../mva
#source /afs/cern.ch/lhcb/software/releases/LBSCRIPTS/LBSCRIPTS_v8r4p2/InstallArea/scripts/SetupProject.sh Gaudi ROOT -v 5.34
sigN=$(grep "initialmassfitABSignalNtwosigma" ../scripts/tables/MassFits.csv | sed -r 's/^[a-zA-Z]*\s+[a-zA-Z]*\s+([0-9\.]+).*/\1/')
bkgN=$(grep "initialmassfitABCombinatorialNtwosigma" ../scripts/tables/MassFits.csv | sed -r 's/^[a-zA-Z]*\s+[a-zA-Z]*\s+([0-9\.]+).*/\1/')
root -l -b <<EOF # wow
.L ../common/lib/libResultDB.so
.x mvaeffs.C+("TMVA.root",${sigN},${bkgN})
.q # I'm so sorry 
EOF
cp -v plots/*.pdf ../latex/figs/
# Store optimal cut
optcut=$(../bin/PrintResult ../scripts/tables/mvaeffs.csv MLPoptcut | grep "^rawvalue.*$" | sed -r 's/^rawvalue\s*:\s*([0-9\.]*.*$)/\1/')
if [ -z $optcut ]
then
	optcut="0.0" # Not great but stops it crashing
	echo "WARNING: optimal cut missing and set to zero"
fi
echo "#!/bin/bash" > ../scripts/mvaopticut.sh
echo "optcut=$optcut" >> ../scripts/mvaopticut.sh
echo "" >> ../scripts/mvaopticut.sh

