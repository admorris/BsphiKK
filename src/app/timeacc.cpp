#include <memory>
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1.h"
#include "TLine.h"
#include "plotmaker.h"
#include "MakeBranchPlot.h"
#include "ResultDB.h"
#define hbar 6.582119514e-4 // eV ps
struct func_info
{
	func_info(std::string _name, std::string _formula, std::string _latex) : name(_name), formula(_formula), latex(_latex) {}
	std::string name, formula, latex;
};
int main (int argc, char const* argv[])
{
	(void)argc; (void)argv;
	std::string file = "ntuples/BsphiKK_MC_mvacut.root";
	std::string branch = "B_s0_TAU*1000";
	std::string weight = "exp(B_s0_TAU/0.001512)";
	std::vector<func_info> functions
	{
		{"power_law","1-1/(1+([1]*(x-[2]))^2)","1 #minus #frac{1}{1 + (#it{b} [#it{t} #minus #it{t}_{0})]^{2}}"},
		{"erf","std::erf([1]*(x-[2]))","erf[#it{b} (#it{t} #minus #it{t}_{0})]"},
		{"tanh","std::tanh([1]*(x-[2]))","tanh[#it{b} (#it{t} #minus #it{t}_{0})]"},
		{"atan","std::atan([1]*(x-[2]))*2/TMath::Pi()","#frac{2}{#it{#pi}}tan^{#minus1}[#it{b} (#it{t} #minus #it{t}_{0})]"}
	};
	double tlow = 0;
	double thi = 10;
	int nbins = 50;
	for(const auto& function: functions)
	{
		// Get the data
		auto hist = std::unique_ptr<TH1D>(MakeBranchPlot(file.c_str(),branch.c_str(),"",weight.c_str(),tlow,thi,nbins));
		// Test the formula
		std::string formula = "x>[2]?[0]*("+function.formula+"):0";
		TFormula test;
		if(test.Compile(formula.c_str()) != 0)
			continue;
		// Construct the fit function
		TF1 fitfunc("fitfunc",formula.c_str(),tlow,thi);
		fitfunc.SetParNames("A","b","t_0");
		fitfunc.SetParameters(1,2,0.1);
		// Perform the fit
		hist->Fit(&fitfunc);
		// Scale everything so it asymptotes to 1
		double norm = fitfunc.GetParameter(0);
		fitfunc.SetParameter(0,1);
		hist = std::unique_ptr<TH1D>(MakeBranchPlot(file.c_str(),branch.c_str(),"",weight.c_str(),tlow,thi,nbins));
		hist->Scale(1/norm);
		// Plot the data
		plotmaker<TH1> plot(hist.get());
		plot.SetTitle("Decay time","ps");
		TCanvas* canv = plot.Draw("E1"); // The plotmaker class retains ownership of this TCanvas and will delete it on destruction
		hist->GetYaxis()->SetTitle("Time acceptance (arbitrary units)");
		// Overlay the fit function
		fitfunc.SetLineStyle(2);
		fitfunc.SetLineColor(2);
		fitfunc.Draw("Lsame");
		// Figure out where acceptance is 50%
		double thalf = fitfunc.GetParameter(2);
		for(; fitfunc.Eval(thalf) < 0.5; thalf+=1e-7) {}
		std::cout << "ε(" << thalf << " ps) = " << fitfunc.Eval(thalf) << std::endl;
		std::cout << "ε(" << thalf/hbar << " eV^−1) = " << fitfunc.Eval(thalf) << std::endl;
		// Draw a vertical line where acceptance is 50%
		TLine line(thalf, 0, thalf, 0.5);
		line.Draw("same");
		line.SetLineStyle(2);
		// Draw some information
		TLatex blurb;
		blurb.DrawLatex(2,0.6,("#font[132]{#it{#varepsilon}(#it{t}) = "+function.latex+"}").c_str());
		blurb.DrawLatex(2,0.3,("#font[132]{#it{#varepsilon}(#it{t}) = 0.5 at #it{t} = " + rdb::tostring(rdb::roundDP(thalf,2),2) + " ps}").c_str());
		// Save the plot
		canv->SaveAs(("timeacc_"+function.name+".pdf").c_str());
	}
	return 0;
}
