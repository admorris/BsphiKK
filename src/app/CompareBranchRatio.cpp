// Standard headers
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TMath.h"
// Custom headers
#include "MakeBranchPlot.h"
#include "plotmaker.h"
using std::string;
void CompareBranchRatio(string Dfilename, string Nfilename, string branchname, string xtitle, string unit, string plotname, string cuts, string Dweight, string Nweight, double xlow, double xup, int nbins)
{
  // Open the files and get the trees
  TH1D*  Dhist = MakeBranchPlot(Dfilename,branchname,cuts,Dweight,xlow,xup,nbins);
  TH1D*  Nhist = MakeBranchPlot(Nfilename,branchname,cuts,Nweight,xlow,xup,nbins);
  Nhist->Divide(Dhist);
  // Draw everything
  plotmaker plotter(Nhist);
  plotter.SetTitle(xtitle, unit);
  TCanvas* plot = plotter.Draw("E1");
  plot->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",(unsigned)120);
  std::string Dfile, Nfile, branch, cuts, xtitle, unit, plot, Dweight, Nweight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"   ,                                                                             "produce help message"          )
    ("Dfile,M"  , value<string>(&Dfile  )->default_value("ntuples/BsphiKK_MC_mva.root"     ), "denominator file"              )
    ("Nfile,R"  , value<string>(&Nfile  )->default_value("ntuples/BsphiKK_data_mva.root"   ), "numerator file"                )
    ("branch,B" , value<string>(&branch )->default_value("KK_M"                            ), "branch to plot"                )
    ("Dweight,w", value<string>(&Dweight)->default_value(""                                ), "denominator weighting variable")
    ("Nweight,W", value<string>(&Nweight)->default_value(""                                ), "numerator weighting variable"  )
    ("cuts,C"   , value<string>(&cuts   )->default_value(""                                ), "optional cuts"                 )
    ("title,T"  , value<string>(&xtitle )->default_value("#it{m}(#it{K^{#plus}K^{#minus}})"), "x-axis title"                  )
    ("unit,U"   , value<string>(&unit   )->default_value("MeV/#it{c}^{2}"                  ), "unit"                          )
    ("plot,O"   , value<string>(&plot   )->default_value("comparison"                      ), "output plot filename"          )
    ("upper,u"  , value<double>(&xup    )->default_value(1800                              ), "branch upper limit"            )
    ("lower,l"  , value<double>(&xlow   )->default_value(980                               ), "branch lower limit"            )
    ("bins,b"   , value<int   >(&nbins  )->default_value(20                                ), "number of bins"                )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  cout << "Entering main function" << endl;
  CompareBranchRatio(Dfile,Nfile,branch,xtitle,unit,plot,cuts,Dweight,Nweight,xlow,xup,nbins);
  return 0;
}
