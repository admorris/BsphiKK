// Apologies in advance for all the hard-coded numbers
#include <iostream>
#include <string>
#include <memory>
#include "ResultDB.h"
#include "datum.h"
#include "TFile.h"
#include "TTree.h"
using plusminus::pm;

datum expectedyield(
	datum NSigFit, // Number of signal events in data
	datum NSigGen, // Number of generated signal events in MC, accounting for generator-level efficiency
	datum NSigSel, // Number of selected signal events in MC
	datum NBkgGen, // Number of generated background events in MC, accounting for generator-level efficiency
	datum NBkgSel, // Number of selected background events in MC
	datum ExtFact) // External factors (branching/fragmentation fractions etc)
{
	return NSigFit*(NSigGen/NSigSel)*(NBkgSel/NBkgGen)*ExtFact;
}
datum GetResult(std::string filename, std::string variablename)
{
	ResultDB table(filename);
	result thisresult = table.Get(variablename);
	return thisresult.value <pm> thisresult.error;
}
datum GetEntries(std::string filename, std::string treename)
{
	double nentries = static_cast<TTree*>(std::unique_ptr<TFile>(TFile::Open(filename.c_str()))->Get(treename.c_str()))->GetEntries();
	return nentries <pm> std::sqrt(nentries);
}
int main (int argc, char const* argv[])
{
	// External stuff
	datum BFBsPhiPhi = 1.84e-5 <pm> 0.18e-5;
	datum BFBdPhiKst = 1e-5 <pm> 0.06e-5;
	datum BFLbPhiKp = BFBsPhiPhi * (3.17e-4 <pm> 0.06e-4) / (1.10e-3 <pm> 0.09e-3); // BF(Bs→ϕϕ) * BF(Λb→J/ψKp) / BF(Bs→J/ψϕ)
	datum BFPhiKK = 0.489 <pm> 0.005;
	datum BFKstKpi = 2. / 3.;
	datum fdfs = 1. / (0.259 <pm> 0.015);
	datum fLbfs = (0.404 <pm> 0.109) * (1. - (0.031 <pm> 0.005) * (10.42 <pm> 0)) / (0.134 <pm> 0.011);
	// Bs to phi phi
	datum NFitBsPhiPhi = GetResult("tables/MassFits.csv", "BsmassfitmvacutlowSignalN");
	datum NGenBsPhiPhi = ((511248.+513999.) <pm> std::sqrt(511248.+513999.)) / (0.167 <pm> 0.004);
	datum NSelBsPhiPhi = GetEntries("../ntuples/Bsphiphi_MC_mvacut.root", "DecayTree");
	// Bd to phi K*
	datum NGenBdPhiKst = ((2020493.+2007492.) <pm> std::sqrt(2020493.+2007492.)) / (0.187 <pm> 0.004);
	datum NSelBdPhiKst = GetEntries("../ntuples/BdphiKst_MC_mvacut.root", "DecayTree");
	// Lb to phi K p
	datum NGenLbPhiKp = ((528968.+523401.) <pm> std::sqrt(528968.+523401.)) / (0.184 <pm> 0.004);
	datum NSelLbPhiKp = GetEntries("../ntuples/LbphiKp_MC_mvacut.root", "DecayTree");
	// Calculate expected yields
	datum NExpBdPhiKst = expectedyield(NFitBsPhiPhi, NGenBsPhiPhi, NSelBsPhiPhi, NGenBdPhiKst, NSelBdPhiKst, (BFBdPhiKst/BFBsPhiPhi)*(BFKstKpi/BFPhiKK)*fdfs);
	datum NExpLbPhiKpguess = expectedyield(NFitBsPhiPhi, NGenBsPhiPhi, NSelBsPhiPhi, NGenLbPhiKp, NSelLbPhiKp, (BFLbPhiKp/BFBsPhiPhi)*(1/BFPhiKK)*fLbfs);
	datum NLbData = GetResult("tables/MassFits.csv", "LbphiKpdatafitLbN");
	datum NLbMC = GetEntries("../ntuples/LbphiKp_MC_mvaVars.root", "DecayTree");
	datum NExpLbPhiKp = NLbData * NSelLbPhiKp/NLbMC;
	std::cout << "Number of expected B⁰→ϕK* events: " << NExpBdPhiKst << std::endl;
	std::cout << "Number of guessed  Λb→ϕKp events: " << NExpLbPhiKpguess << std::endl;
	std::cout << "Number of expected Λb→ϕKp events: " << NExpLbPhiKp << std::endl;
	return 0;
}

