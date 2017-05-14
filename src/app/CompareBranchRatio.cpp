// Standard headers
#include <string>
#include <iostream>
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
void CompareBranchRatio(string Dfilename, string Nfilename, string Dbranchname, string Nbranchname, string xtitle, string unit, string plotname, string Dcuts, string Ncuts, string Dweight, string Nweight, double xlow, double xup, int nbins,std::string blurb)
{
  TH1D*  Dhist = MakeBranchPlot(Dfilename,Dbranchname,Dcuts,Dweight,xlow,xup,nbins);
  TH1D*  Nhist = MakeBranchPlot(Nfilename,Nbranchname,Ncuts,Nweight,xlow,xup,nbins);
  Nhist->Divide(Dhist);
  Nhist->SetMaximum(Nhist->GetMaximum()*1.5);
  Nhist->SetMinimum(0);
  // Draw everything
  plotmaker plotter(Nhist);
  plotter.SetBlurb(blurb);
  plotter.SetTitle(xtitle, unit);
  TCanvas* plot = plotter.Draw("E1");
  plot->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",(unsigned)120);
  string Dfile, Nfile, Dbranch, Nbranch, Dcuts, Ncuts, xtitle, unit, plot, Dweight, Nweight, blurb;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help"   ,                                                                               "produce help message"          )
    ("blurb", value<std::string>(&blurb), "blurb text")
    ("Dfile"  , value<std::string>(&Dfile  )->default_value("ntuples/BsphiKK_MC_mva.root"  ), "denominator file"              )
    ("Nfile"  , value<std::string>(&Nfile  )->default_value("ntuples/BsphiKK_data_mva.root"), "numerator file"                )
    ("Dbranch", value<std::string>(&Dbranch)->default_value("KK_M"                         ), "denominator branch to plot"    )
    ("Nbranch", value<std::string>(&Nbranch)->default_value("KK_M"                         ), "numerator branch to plot"      )
    ("Dweight", value<std::string>(&Dweight)->default_value(""                             ), "denominator weighting variable")
    ("Nweight", value<std::string>(&Nweight)->default_value(""                             ), "numerator weighting variable"  )
    ("Dcuts"  , value<std::string>(&Dcuts  )->default_value(""                             ), "denominator cuts"              )
    ("Ncuts"  , value<std::string>(&Ncuts  )->default_value(""                             ), "numerator cuts"                )
    ("title"  , value<std::string>(&xtitle )->default_value("#it{m}(#it{K^{+}K^{-}})"      ), "x-axis title"                  )
    ("unit"   , value<std::string>(&unit   )->default_value("MeV/#it{c}^{2}"               ), "unit"                          )
    ("plot"   , value<std::string>(&plot   )->default_value("comparison"                   ), "output plot filename"          )
    ("upper"  , value<double     >(&xup    )->default_value(1800                           ), "branch upper limit"            )
    ("lower"  , value<double     >(&xlow   )->default_value(980                            ), "branch lower limit"            )
    ("bins"   , value<int        >(&nbins  )->default_value(20                             ), "number of bins"                )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }
  std::cout << "Entering main function" << std::endl;
  CompareBranchRatio(Dfile,Nfile,Dbranch,Nbranch,xtitle,unit,plot,Dcuts,Ncuts,Dweight,Nweight,xlow,xup,nbins,blurb);
  return 0;
}
