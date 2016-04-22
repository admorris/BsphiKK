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

void FitBranch(string filename, string branchname, string modelname, string xtitle, string unit, string plotname, string cuts, string weight, double xlow, double xup, int nbins, bool drawpulls)
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
  FitModel.AddComponent("model",modelname);
  FitModel.Fit(data);
  std::cout << "Plotting" << endl;
  data->plotOn(frame,Binning(nbins));
  FitModel.Plot(frame);
  frame->SetMaximum(frame->GetMaximum()*1.3);
  plotmaker* plotter;
  if(drawpulls)
  {
    RooHist* pullhist = frame->pullHist();
    RooPlot* pullframe = x->frame(Title("Pull"));
    pullframe->addPlotable(pullhist,"B");
    plotter = new plotmaker(frame,pullframe);
  }
  else
  {
    plotter = new plotmaker(frame);
  }
  plotter->SetTitle(xtitle, unit);
  plotter->Draw()->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  std::string file, branch, model, cuts, xtitle, unit, plot, weight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                                 "produce help message"                  )
    ("pulls,P" ,                                                                                 "plot with pulls"                       )
    ("file,F"  , value<string>(&file  )->default_value("ntuples/BsphiKK_data_mva.root"        ), "data file"                             )
    ("branch,B", value<string>(&branch)->default_value("B_s0_LOKI_Mass"                       ), "branch to plot"                        )
    ("model,M" , value<string>(&model )->default_value("Gaussian"                             ), "model to use in fit"                   )
    ("weight,W", value<string>(&weight)->default_value(""                                     ), "weighting variable"                    )
    ("cuts,C"  , value<string>(&cuts  )->default_value(""                                     ), "optional cuts"                         )
    ("title,T" , value<string>(&xtitle)->default_value("#it{m}(#it{#phi K^{#plus}K^{#minus}})"), "x-axis title (takes ROOT LaTeX format)")
    ("unit,U"  , value<string>(&unit  )->default_value("MeV/#it{c}^{2}"                       ), "unit (takes ROOT LaTeX format)"        )
    ("plot,O"  , value<string>(&plot  )->default_value("plottedbranch"                        ), "output plot filename"                  )
    ("upper,u" , value<double>(&xup   )->default_value(5600                                   ), "branch upper limit"                    )
    ("lower,l" , value<double>(&xlow  )->default_value(5200                                   ), "branch lower limit"                    )
    ("bins,b"  , value<int   >(&nbins )->default_value(50                                     ), "number of bins"                        )
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
  FitBranch(file,branch,model,xtitle,unit,plot,cuts,weight,xlow,xup,nbins,vmap.count("pulls"));
  return 0;
}
