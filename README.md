# Angular analysis of the decay Bs→ϕK+K−

Please see the TWiki page:  
https://twiki.cern.ch/twiki/bin/viewauth/LHCbPhysics/Bs2PhiKK

## Dependencies
- ROOT 6.04.14  
- My 'common' libraries: https://github.com/abmorris/common  
- My fork of RapidFit: https://github.com/abmorris/RapidFit  
## Getting the nTuples
For the collision data and full MC ntuples, it should be enough to run `ganga Ganga-DaVinci.py` from the folder `gridjobs/DaVinci/AllnTuples/`  
For the generator-level MC, see `gridjobs/Gauss/PhaseSpaceMC/Ganga-Gauss.py`  
The file scripts/getoutputdata.sh relies on there being a file called `LFNs.txt` in each job directory
## Running the selection
Should be as simple as running `scripts/ANALYSIS.sh` from the line containing `merge.sh` downwards
## Acceptance and Background
## Angular fit
From the `fits/` folder run `fitting -f Bs2PhiKKTotalFitToData.xml`
