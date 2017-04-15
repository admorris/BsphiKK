// Self
#include "LegendreMomentPlot.h"
// Std
#include <iostream>
// GSL
#include <gsl/gsl_sf_legendre.h>

LegendreMomentPlot::LegendreMomentPlot(std::string name, int _order, int nbins, double xlow, double xup) : order(_order)
{
  name += "<P_{" + std::to_string(order) + "}>";
  std::cout << name << std::endl;
  hist = TH1D(name.c_str(),"",nbins,xlow,xup);
}
void LegendreMomentPlot::Fill(const double& mass, const double& angle, const double weight)
{
  double p = gsl_sf_legendre_Pl(order, angle);
  hist.Fill(mass,p*weight);
}
std::vector<LegendreMomentPlot> LegendreMomentPlot::FillPlotsFromTree(std::string name, TTree& tree, std::string massbranch, std::string ct2branch, std::string weightname, std::string xtitle, std::string unit, std::string plotname, double xlow, double xup, int nbins, int max_order, double weightmodifier)
{
  std::vector<LegendreMomentPlot> plots;
  for(int order = 0; order <= max_order; order++)
    plots.push_back(LegendreMomentPlot(name, order, nbins, xlow, xup));
  float weight = 1; // RooStats s-plot tends to add floats to a TTree
  if(weightname != "")
    tree.SetBranchAddress(weightname.c_str(),&weight);
  double mass; tree.SetBranchAddress(massbranch.c_str(),&mass);
  double ct2 ; tree.SetBranchAddress(ct2branch .c_str(),&ct2 );
  for(int i = tree.GetEntries(); i-->0;)
  {
    tree.GetEntry(i);
    for(auto& lmplot : plots)
      lmplot.Fill(mass,ct2,weight*weightmodifier);
  }
  return plots;
}

