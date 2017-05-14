// Standard headers
#include <string>
#include <vector>
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
#include "ResultDB.h"

void FitBranch(std::string filename, std::string branchname, std::vector<std::string> model, std::string xtitle, std::string unit, std::string plotname, std::string cuts, std::string weight, double xlow, double xup, int nbins, bool drawpulls, std::string resname, std::string DBfilename,std::string blurb)
{
  TFile* file = TFile::Open(filename.c_str());
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
  std::vector<Component*> components;
  int nentries = tree->GetEntries(cuts.c_str());
  for(std::string modelname: model)
  {
    RooRealVar* N = new RooRealVar("N","",nentries/model.size(),0,nentries);
    components.push_back(FitModel.AddComponent(modelname,modelname,N));
  }
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
    plotter = new plotmaker(frame);
    plotter->SetPullPlot(pullframe);
  }
  else
  {
    plotter = new plotmaker(frame);
  }
  plotter->SetBlurb(blurb);
  plotter->SetTitle(xtitle, unit);
  plotter->Draw()->SaveAs((plotname+".pdf").c_str());
  std::vector<parameter> pars;
  RooAbsArg* arg;
  for(auto Model: components)
  {
    RooFIter iterator = Model->GetParameters(data)->fwdIterator();
    while((arg = iterator.next()))
    {
      std::string name = arg->GetName();
      std::string modelname = Model->GetName();
      size_t pos = name.find(modelname);
      pars.push_back(parameter(name.substr(pos+modelname.length()),arg->GetTitle(),Model));
    }
  }
/******************************************************************************/
  if(resname!="")
  {
    ResultDB table(DBfilename);
    for(auto par : pars)
    {
      table.Update(resname+par.safename(),"N",par.value,par.error);
    }
    table.Save();
    cout << "Results saved to " << DBfilename << endl;
  }
  for(auto par : pars)
  {
    cout << "$" << par.latex << "$ & $" << par.value << " \\pm " << par.error << "$ \\\\" << endl;
  }
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string file, branch, cuts, xtitle, unit, plot, weight, dbf, resname, blurb;
  std::vector<std::string> model;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                                      "produce help message"                  )
    ("blurb", value<std::string>(&blurb), "blurb text")
    ("pulls,P" ,                                                                                      "plot with pulls"                       )
    ("file,F"  , value<std::string>(&file  )->default_value("ntuples/BsphiKK_data_mva.root"        ), "data file"                             )
    ("branch,B", value<std::string>(&branch)->default_value("B_s0_LOKI_Mass"                       ), "branch to plot"                        )
    ("model,M" , value<std::vector<std::string>>(&model )->multitoken(                             ), "model to use in fit"                   )
    ("weight,W", value<std::string>(&weight)->default_value(""                                     ), "weighting variable"                    )
    ("cuts,C"  , value<std::string>(&cuts  )->default_value(""                                     ), "optional cuts"                         )
    ("title,T" , value<std::string>(&xtitle)->default_value("#it{m}(#it{#phi K^{#plus}K^{#minus}})"), "x-axis title (takes ROOT LaTeX format)")
    ("unit,U"  , value<std::string>(&unit  )->default_value("MeV/#it{c}^{2}"                       ), "unit (takes ROOT LaTeX format)"        )
    ("plot,O"  , value<std::string>(&plot  )->default_value("plottedbranch"                        ), "output plot filename"                  )
    ("upper,u" , value<double>(&xup   )->default_value(5600                                        ), "branch upper limit"                    )
    ("lower,l" , value<double>(&xlow  )->default_value(5200                                        ), "branch lower limit"                    )
    ("bins,b"  , value<int   >(&nbins )->default_value(50                                          ), "number of bins"                        )
    ("output-file" , value<std::string>(&dbf    )->default_value("FitResults.csv"                  ), "output file"                           )
    ("save-results", value<std::string>(&resname)->default_value(""                                ), "name to save results as"               )
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
  FitBranch(file,branch,model,xtitle,unit,plot,cuts,weight,xlow,xup,nbins,vmap.count("pulls"),resname,dbf,blurb);
  return 0;
}
