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
#include "MassFitter.h"
#include "GetTree.h"

void FitBranch(string filename, string branchname, string modelname, string xtitle, string unit, string plotname, string cuts, string weight, double xlow, double xup, int nbins)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = GetTree(file,cuts);
  using namespace RooFit;
  RooRealVar* x = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
  RooRealVar* w;
  RooDataSet* data;
  std::cout << "Importing tree" << endl;
  if(weight!="")
  {
    w = new RooRealVar(weight.c_str(),"",-0.5,1.5);
    data = new RooDataSet("data","",RooArgSet(*x,*w),WeightVar(*w),Import(*tree));
  }
  else
  {
    data = new RooDataSet("data","",RooArgSet(*x),Import(*tree));
  }
  RooPlot* frame = x->frame();
  MassFitter FitModel(x);
  FitModel.SetPDF(modelname,"none");
  FitModel.Fit(data);
  std::cout << "Plotting" << endl;
  data->plotOn(frame,Binning(nbins));
  FitModel.Plot(frame);
  frame->SetMaximum(frame->GetMaximum()*1.3);
  plotmaker plotter(frame);
  plotter.SetTitle(xtitle, unit);
  plotter.Draw()->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string file, branch, model, cuts, xtitle, unit, plot, weight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                                      "produce help message"                      )
    ("file,F"  , value<std::string>(&file  )->default_value("ntuples/BsphiKK_data_mva.root"        ), "set data file"                             )
    ("branch,B", value<std::string>(&branch)->default_value("B_s0_LOKI_Mass"                       ), "set branch to plot"                        )
    ("model,M" , value<std::string>(&branch)->default_value("Gaussian"                             ), "set model to use in fit"                   )
    ("weight,W", value<std::string>(&weight)->default_value(""                                     ), "set weighting variable"                    )
    ("cuts,C"  , value<std::string>(&cuts  )->default_value(""                                     ), "set optional cuts"                         )
    ("title,T" , value<std::string>(&xtitle)->default_value("#it{m}(#it{#phi K^{#plus}K^{#minus}})"), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"  , value<std::string>(&unit  )->default_value("MeV/#it{c}^{2}"                       ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"  , value<std::string>(&plot  )->default_value("plottedbranch"                        ), "set output plot filename"                  )
    ("upper,u" , value<double     >(&xup   )->default_value(5600                                   ), "set branch upper limit"                    )
    ("lower,l" , value<double     >(&xlow  )->default_value(5200                                   ), "set branch lower limit"                    )
    ("bins,b"  , value<int        >(&nbins )->default_value(50                                     ), "set number of bins"                        )
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
  FitBranch(file,branch,model,xtitle,unit,plot,cuts,weight,xlow,xup,nbins);
  return 0;
}
