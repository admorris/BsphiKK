// Custom headers
#include "GetTree.h"
#include "MakeBranchPlot.h"
using std::string;
TH1D* MakeBranchPlot(string filename, string branchname, string cuts, string weight, double xlow, double xup, int nbins)
{
  TTree* tree = GetTree(filename,cuts);
  TH1D* hist = new TH1D((branchname+"hist").c_str(),"",nbins,xlow,xup);
  tree->Draw((branchname+">>"+hist->GetName()).c_str(),weight.c_str(),"goff");
  hist->SetLineColor(kBlack);
  hist->SetMarkerStyle(20);
  return hist;
}
