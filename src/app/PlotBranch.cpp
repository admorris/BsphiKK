// Standard headers
#include <string>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// RooFit headers
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooRealVar.h"
// Custom headers
#include "plotmaker.h"

void PlotBranch(string filename, string branchname, string xtitle, string unit, string plotname, string cuts, double xlow, double xup)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  using namespace RooFit;
  RooRealVar* x = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
  std::cout << "Importing tree" << endl;
  RooDataSet* data = new RooDataSet("data","",RooArgSet(*x),Import(*tree),cuts.c_str());
  RooPlot* frame = x->frame();
  std::cout << "Plotting" << endl;
  data->plotOn(frame);
  plotmaker plotter(frame);
  plotter.SetTitle(xtitle, unit);
  plotter.Draw()->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options");
  std::string file, branch, cuts, xtitle, unit, plot;
  double xlow, xup;
  desc.add_options()
    ("help,H"  ,                                                                                                    "produce help message"                      )
    ("file,F"  , value<std::string>(&file  )->default_value("ntuples/BsphiKK_data_mva.root"                      ), "set data file"                             )
    ("branch,B", value<std::string>(&branch)->default_value("B_s0_LOKI_Mass"                                     ), "set branch to plot"                        )
    ("cuts,C"  , value<std::string>(&cuts  )->default_value(""                                                   ), "set optional cuts"                         )
    ("title,T" , value<std::string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}K^{#plus}K^{#minus}})"), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"  , value<std::string>(&unit  )->default_value("MeV/#it{c}^{2}"                                     ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"  , value<std::string>(&plot  )->default_value("plottedbranch"                                      ), "set output plot filename"                  )
    ("upper,u" , value<double     >(&xup   )->default_value(5600                                                 ), "set branch upper limit"                    )
    ("lower,l" , value<double     >(&xlow  )->default_value(5200                                                 ), "set branch lower limit"                    )
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
  PlotBranch(file,branch,xtitle,unit,plot,cuts,xlow,xup);
  return 0;
}
