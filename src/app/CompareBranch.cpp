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
#include "GetTree.h"

void CompareBranch(string MCfilename, string REfilename, string branchname, string xtitle, string unit, string plotname, string cuts, string MCweight, string REweight, double xlow, double xup, int nbins)
{
  // Open the files and get the trees
  TH1D*  MChist = MakeBranchPlot(MCfilename,branchname,cuts,MCweight,xlow,xup,nbins);
  TH1D*  REhist = MakeBranchPlot(REfilename,branchname,cuts,REweight,xlow,xup,nbins);
  MChist->Scale(1./MChist->Integral());
  REhist->Scale(1./REhist->Integral());
  MChist->SetDrawOption("B");
  MChist->SetFillColor(kOrange);
  MChist->SetLineColor(kOrange);
  REhist->SetLineColor(kBlack);
  REhist->SetMarkerStyle(20);
  MChist->SetMaximum(MChist->GetMaximum()*1.3);
  MChist->SetMinimum(0);
  // Draw everything
  plotmaker plotter(MChist);
  plotter.SetTitle(xtitle, unit);
  TCanvas* plot = plotter.Draw();
  REhist->Draw("sameE1");
  plot->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",(unsigned)120);
  std::string MCfile, REfile, branch, cuts, xtitle, unit, plot, MCweight, REweight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"    ,                                                                                        "produce help message"                      )
    ("MCfile,M"  , value<std::string>(&MCfile  )->default_value("ntuples/BsphiKK_MC_mva.root"          ), "set Monte Carlo file"                      )
    ("REfile,R"  , value<std::string>(&REfile  )->default_value("ntuples/BsphiKK_data_mva.root"        ), "set collision data file"                   )
    ("branch,B"  , value<std::string>(&branch  )->default_value("B_s0_LOKI_Mass"                       ), "set branch to plot"                        )
    ("MCweight,w", value<std::string>(&MCweight)->default_value(""                                     ), "set Monte Carlo weighting variable"        )
    ("REweight,W", value<std::string>(&REweight)->default_value(""                                     ), "set collision data weighting variable"     )
    ("cuts,C"    , value<std::string>(&cuts    )->default_value(""                                     ), "set optional cuts (UNIMPLEMENTED)"         )
    ("title,T"   , value<std::string>(&xtitle  )->default_value("#it{m}(#it{#phi K^{#plus}K^{#minus}})"), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"    , value<std::string>(&unit    )->default_value("MeV/#it{c}^{2}"                       ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"    , value<std::string>(&plot    )->default_value("comparison"                           ), "set output plot filename"                  )
    ("upper,u"   , value<double     >(&xup     )->default_value(5600                                   ), "set branch upper limit"                    )
    ("lower,l"   , value<double     >(&xlow    )->default_value(5200                                   ), "set branch lower limit"                    )
    ("bins,b"    , value<int        >(&nbins   )->default_value(20                                     ), "set number of bins"                        )
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
  CompareBranch(MCfile,REfile,branch,xtitle,unit,plot,cuts,MCweight,REweight,xlow,xup,nbins);
  return 0;
}
