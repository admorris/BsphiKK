# XML files for Bs→ϕKK amplitude fits with RapidFit

## Introduction

To avoid maintaining lots of duplicated XML tags, the RapidFit configs are built from modular XML snippets stored in `modules/` according to lists stored in `src/`. The script `modules/assemble.sh` does most of the work and has some knowledge of what RapidFit requires. To build the RapidFit configs, just type `make` in the `modules/` directory. There are three folders for different types of fit we want to do, which are mirrored in the `src/` folder: `toystudies/`, `datafits/` and `mcfits/` (which is sub-divided into `pwave/` and `phasespace/`). These can be extended or modified by changing the folder structure inside `src/` although at the moment you'll have to create the new folders yourself.

## Acceptance

A special `acceptance.xml` file can be used in conjunction with `../scripts/angacc.sh` to generate the Legendre coefficients for a nice, smooth acceptance function. The location of the outputted file must be given in `modules/pdf/moments_acceptance.xml`.

Alternatively, `../scripts/angacc_hist.sh` will generate an adaptively-binned histogram containing the acceptance. The output of this must be given in `modules/pdf/histogram_acceptance.xml`.

## Additional output

In the `output/` folder there are two scripts: `compareresults.sh`, which compares the p-wave parameters to 3 previous analysis (CDF, LHCb 2011 and LHCb 2011+2012)

## Toy studies

Toy studies allow us to check for biases in the PDF, e.g. in the normalisation. RapidFit will generate toys from a PDF and then perform a fit to them. You should check that the fit results are consistent with the starting values and the pull distriubutions are Gaussians with a mean of zero and width one.

- Simple phase space model just draws the function and generates points (no fit)
- P-wave can test fitting for amplitudes, resonance parameters and delta Gamma
- Analytical P-wave PDF for comparison

## Monte Carlo fits

The Monte Carlo fits allow for the PDF to be compared with the distribution produced by EvtGen and to test the acceptance function. This is split into two subfolders `phasespace/` and `pwave/`

- Generator-level phase space sample: verify the PDF without calculating amplitudes
- Fully-selected phase space sample: verify the acceptance function
- Generator-level P-wave sample: check agreement between this PDF and EvtGen
 - Starting parameters are the CDF results
 - Only one phase can be fitted for
- Fully-selected P-wave sample: check the CDF result still comes out when applying acceptance

## Data fits

The data fits establish which resonance model best describes the data and gives the results for the analysis.

**NB:** remember that, until another spin>0 resonance is added, the fit cannot be sensitive to δ⟂

- Fit to the region 15 MeV around the ϕ mass and compare with the full Run1 Bs→ϕϕ angular analysis
- Extend the fit region from threshold to 1.08 GeV (where EvtGen cuts off the ϕ tail) and decide the best model for that
- Extend the fit region from threshold to 1.5 GeV and decide the best model for the region between the ϕ and the f2´(1525)
- Keep the resonant components fount in the last step and extend the fit region to the full 1.8 GeV and decide the best model for the region above the f2´(1525)

# Running the fits locally

The script `runall.sh` will build the XML files and start executing them in the current session.
It assumes that RapidFit is already built and that the `fitting` binary can be found under `$PATH`.
It takes 3 optional arguments: the first restricts it to running the fits in a particular folder (e.g. `datafits/`), the second is a pattern match of which XML files to run (e.g. `"1800*LHCb"`) and the third is any other command line options you may want to pass to `fitting`.

# Running the fits at Edinburgh

Below are instructions of how to run on either ECDF/Eddie or the SOPA batch system.


Copy the appropriate `RFjobconfig.[HOST].default.sh` file to `RFjobconfig.sh`

The script `submitall.sh` takes the same syntax as `runall.sh`, but will submit jobs to the batch system.
Job runtimes can be configured via the `.list` files using the `.sh` snippets found in `modules/joboptions`.
In principle, this can be extended to other SGE/OGE options.

The script `stageout.sh` can be used to move the data to the PPE DataStore area when running on Eddie.
Please modify it to use your own area.
