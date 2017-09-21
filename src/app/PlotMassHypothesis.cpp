#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include "GetTree.h"
#include "boost/program_options.hpp"
#include "plotmaker.h"
#include "TH1.h"
#include "TLorentzVector.h"

int main(int argc, char* argv[])
{
	std::vector<std::string> names;
	std::vector<double> masses;
	std::string PXb, PYb, PZb, file, cuts, xtitle, unit, plotname;
	double xup, xlow;
	int nbins;
	using namespace boost::program_options;
	options_description desc("Allowed options",120);
	desc.add_options()
		("help,H", "produce help message")
		("particles", value<std::vector<std::string>>(&names)->multitoken(), "particles")
		("masses", value<std::vector<double>>(&masses)->multitoken(), "masses")
		("PXbranch", value<std::string>(&PXb)->default_value("_PX"), "PX branch name")
		("PYbranch", value<std::string>(&PYb)->default_value("_PY"), "PY branch name")
		("PZbranch", value<std::string>(&PZb)->default_value("_PZ"), "PZ branch name")
		("file", value<std::string>(&file)->required(), "data ntuple")
		("cuts", value<std::string>(&cuts), "set optional cuts")
		("title", value<std::string>(&xtitle), "x-axis title (takes ROOT LaTeX format)")
		("unit", value<std::string>(&unit)->default_value("MeV/#it{c}^{2}"), "x-axis unit (takes ROOT LaTeX format)")
		("plot", value<std::string>(&plotname)->default_value("mass"), "output plot filename")
		("upper", value<double>(&xup), "x-axis upper limit")
		("lower", value<double>(&xlow), "x-axis lower limit")
		("bins", value<int>(&nbins)->default_value(50), "number of bins")
	;
	variables_map vmap;
	store(parse_command_line(argc, argv, desc), vmap);
	notify(vmap);
	if (vmap.count("help"))
	{
		std::cout << desc << std::endl;
		return 1;
	}
	bool defaultup = vmap.count("upper") == 0;
	bool defaultlow = vmap.count("lower") == 0;
	if(defaultup || defaultlow)
	{
		if(defaultlow)
			xlow = std::accumulate(masses.begin(), masses.end(), 0.);
		if(defaultup)
			xup = 10.*std::accumulate(masses.begin(), masses.end(), 0.);
	}
	TH1D hist("hist", "", nbins, xlow, xup);
	TTree* tree = GetTree(file, cuts);
	std::cout << tree->GetName() << std::endl;
	const unsigned np = masses.size();
	double PX[np], PY[np], PZ[np];
	std::cout << "Setting branch addresses" << std::endl;
	for(unsigned i = 0; i < np; i++)
	{
		tree->SetBranchAddress((names[i]+PXb).c_str(), &PX[i]);
		tree->SetBranchAddress((names[i]+PYb).c_str(), &PY[i]);
		tree->SetBranchAddress((names[i]+PZb).c_str(), &PZ[i]);
	}
	unsigned nevts = tree->GetEntries();
	std::cout << "Looping over " << nevts << " events" << std::endl;
	for(unsigned i = 0; i < nevts; i++)
	{
		tree->GetEntry(i);
		TLorentzVector combination;
		for(unsigned j = 0; j < np; j++)
		{
			TLorentzVector P;
			P.SetXYZM(PX[j], PY[j], PZ[j], masses[j]);
			combination += P;
		}
		hist.Fill(combination.M());
	}
	plotmaker<TH1> plotter(&hist);
	plotter.SetTitle(xtitle, unit);
	TCanvas* plot = plotter.Draw("E1");
	plot->SaveAs((plotname+".pdf").c_str());
	return 0;
}
