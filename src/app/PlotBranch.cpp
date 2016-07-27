// Standard headers
#include <string>
#include <iostream>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TH1.h"
// Custom headers
#include "plotmaker.h"
#include "MakeBranchPlot.h"
using std::string;
using std::cout;
using std::endl;
void PlotBranch(string filename, string branchname, string xtitle, string unit, string plotname, string cuts, string weight, double xlow, double xup, int nbins, string overlayfilename, double scale)
{
  TH1D* frame = MakeBranchPlot(filename, branchname, cuts, weight, xlow, xup, nbins);
  frame->SetMaximum(frame->GetMaximum()*1.3);
  plotmaker plotter(frame);
  plotter.SetTitle(xtitle, unit);
  TCanvas* plot = plotter.Draw("E1");
  if(overlayfilename!="")
  {
    TH1D* overlay = MakeBranchPlot(overlayfilename, branchname, cuts, weight, xlow, xup, nbins);
    overlay->Scale(scale);
    overlay->SetDrawOption("B");
    overlay->SetFillColor(kOrange);
    overlay->SetLineColor(kOrange);
    overlay->Draw("same");
  }
  plot->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  string file, branch, cuts, xtitle, unit, plot, weight, overlay;
  double xlow, xup, scale;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                                  "produce help message"                      )
    ("file,F"  , value<string>(&file   )->default_value("ntuples/BsphiKK_data_mva.root"        ), "set data file"                             )
    ("branch,B", value<string>(&branch )->default_value("B_s0_LOKI_Mass"                       ), "set branch to plot"                        )
    ("weight,W", value<string>(&weight )->default_value(""                                     ), "set weighting variable"                    )
    ("cuts,C"  , value<string>(&cuts   )->default_value(""                                     ), "set optional cuts"                         )
    ("title,T" , value<string>(&xtitle )->default_value("#it{m}(#it{#phi K^{#plus}K^{#minus}})"), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"  , value<string>(&unit   )->default_value("MeV/#it{c}^{2}"                       ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"  , value<string>(&plot   )->default_value("plottedbranch"                        ), "set output plot filename"                  )
    ("upper,u" , value<double>(&xup    )->default_value(5600                                   ), "set branch upper limit"                    )
    ("lower,l" , value<double>(&xlow   )->default_value(5200                                   ), "set branch lower limit"                    )
    ("bins,b"  , value<int   >(&nbins  )->default_value(50                                     ), "set number of bins"                        )
    ("overlay" , value<string>(&overlay)->default_value(""                                     ), "file to overlay the same branch from"      )
    ("scale"   , value<double>(&scale  )->default_value(1                                      ), "scale factor for the overlaid branch"      )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    cout << desc << endl;
    return 1;
  }
  cout << "Entering main function" << endl;
  PlotBranch(file,branch,xtitle,unit,plot,cuts,weight,xlow,xup,nbins,overlay,scale);
  return 0;
}
