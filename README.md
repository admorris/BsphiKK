# Angular analysis of the decay Bs→ϕK+K−

Please see the TWiki page:  
https://twiki.cern.ch/twiki/bin/viewauth/LHCbPhysics/Bs2PhiKK

## Dependencies
- ROOT 6.06.02  
- My 'common' libraries: https://github.com/abmorris/common  
 - Included as a git submodule for this project. After cloning, you must do `git submodule init` then `git submodule update`  
- My fork of RapidFit: https://github.com/abmorris/RapidFit  
 - `scripts/angacc.sh` and `fits/runall.sh` assume that `RapidFit/bin/fitting` is in `$PATH`  

## Getting the nTuples
For the collision data and full MC ntuples, it should be enough to run `ganga Ganga-DaVinci.py` from the folder `gridjobs/DaVinci/AllnTuples/`  
For the generator-level MC, see `gridjobs/Gauss/PhaseSpaceMC/Ganga-Gauss.py`  
The file scripts/getoutputdata.sh relies on there being a file called `LFNs.txt` in each job directory

## Running the selection
Should be as simple as running `scripts/ANALYSIS.sh` from the line containing `merge.sh` downwards

## Acceptance
The output files of `scripts/angacc.sh` and `scripts/angacc_hist.sh` should be pointed to by `fits/modules/pdf/moments_acceptance.xml` and `fits/modules/pdf/histogram_acceptance.xml` respectively

## Angular fit
The script `fits/runall.sh` will assemble the RapidFit XML files and perform fits. It can take arguments to specify which fits should be run. See `fits/README.md` for more details.

