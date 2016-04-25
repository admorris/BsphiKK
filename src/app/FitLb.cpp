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
#include "GetData.h"

void FitLb(string filename, string Sfilename, string Bfilename, string branchname, string modelname, string xtitle, string unit, string plotname, string cuts, double xlow, double xup, int nbins, bool drawpulls)
{
  using namespace RooFit;
  RooRealVar* x = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
  std::cout << "Importing trees" << endl;
  RooDataSet* Cdata = GetData("collision",filename,cuts,x)
           ,* Sdata = GetData("Lb",Sfilename,cuts,x)
           ,* Bdata = GetData("Bs",Bfilename,cuts,x)
           ;
  std::cout << "Done" << endl;
  RooPlot* Cframe = x->frame()
        ,* Sframe = x->frame()
        ,* Bframe = x->frame()
        ;
  MassFitter FitModel(x);
  plotmaker* plotter;
  RooRealVar NLb("N","",100,0,10000);
  RooRealVar NBs("N","",100,0,100000);
  Component* SigMod = FitModel.AddComponent("Lb",modelname,&NLb);
  SigMod->SetRange("mean",xlow,xup);
  SigMod->SetValue("mean",5619.5);
  SigMod->FixShapeTo(Sdata);
  Sdata->plotOn(Sframe,Binning(nbins));
  SigMod->GetPDF()->plotOn(Sframe);
  plotter = new plotmaker(Sframe);
  plotter->SetTitle(xtitle, unit);
  plotter->Draw()->SaveAs((plotname+"S.pdf").c_str());
  SigMod->FloatPar("mean");
  Component* BkgMod = FitModel.AddComponent("Bs","Exponential",&NBs);
  BkgMod->FixShapeTo(Bdata);
  Bdata->plotOn(Bframe,Binning(nbins));
  BkgMod->GetPDF()->plotOn(Bframe);
  plotter = new plotmaker(Bframe);
  plotter->SetTitle(xtitle, unit);
  plotter->Draw()->SaveAs((plotname+"B.pdf").c_str());
  FitModel.Fit(Cdata);
  std::cout << "Plotting" << endl;
  Cdata->plotOn(Cframe,Binning(nbins));
  FitModel.Plot(Cframe);
  Cframe->SetMaximum(Cframe->GetMaximum()*1.3);
  plotter = new plotmaker(Cframe);
  plotter->SetTitle(xtitle, unit);
  plotter->Draw()->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  std::string file, sfile, bfile, branch, model, cuts, xtitle, unit, plot;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                             "produce help message")
    ("pulls,P" ,                                                                             "plot with pulls"     )
    ("file,F"  , value<string>(&file  )->default_value("ntuples/BsphiKK_data_mvaVars.root"), "data file"           )
    ("Sfile"   , value<string>(&sfile )->default_value("ntuples/LbphiKp_MC_mvaVars.root"  ), "signal MC file"      )
    ("Bfile"   , value<string>(&bfile )->default_value("ntuples/BsphiKK_MC_mvaVars.root"  ), "background MC file"  )
    ("branch,B", value<string>(&branch)->default_value("phiKpM"                           ), "branch to plot"      )
    ("model,M" , value<string>(&model )->default_value("Crystal Ball + 2 Gaussians"       ), "model to use in fit" )
    ("cuts,C"  , value<string>(&cuts  )->default_value("KK_M<1800"                        ), "optional cuts"       )
    ("title,T" , value<string>(&xtitle)->default_value("#it{m}(#it{#phi Kp})"             ), "x-axis title"        )
    ("unit,U"  , value<string>(&unit  )->default_value("MeV/#it{c}^{2}"                   ), "unit"                )
    ("plot,O"  , value<string>(&plot  )->default_value("plottedbranch"                    ), "output plot filename")
    ("upper,u" , value<double>(&xup   )->default_value(6000                               ), "branch upper limit"  )
    ("lower,l" , value<double>(&xlow  )->default_value(5500                               ), "branch lower limit"  )
    ("bins,b"  , value<int   >(&nbins )->default_value(50                                 ), "number of bins"      )
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
  FitLb(file,sfile,bfile,branch,model,xtitle,unit,plot,cuts,xlow,xup,nbins,vmap.count("pulls"));
  return 0;
}
