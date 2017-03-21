#pragma once
// Standard C++
#include <string>
// ROOT
#include "TH1D.h"
#include "TTree.h"
struct LegendreMomentPlot
{
  LegendreMomentPlot(int _order, int nbins, double xlow, double xup);
  int order;
  TH1D hist;
  void Fill(const double& mass, const double& angle, const double weight = 1.);
  static std::vector<LegendreMomentPlot> FillPlotsFromTree(TTree& tree, std::string massbranch, std::string ct2branch, std::string weightname, std::string xtitle, std::string unit, std::string plotname, double xlow, double xup, int nbins, int max_order, double weightmodifier);
};

