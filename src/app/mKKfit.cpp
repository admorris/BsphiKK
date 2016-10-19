// Standard headers
#include <string>
#include <iostream>
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
#include "MassFitter.h"
#include "plotmaker.h"
#include "GetTree.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;
void mKKfit(string filename, string branchname, string cuts, string weight, string xtitle, string unit, string plotname, double xlow, double xup, double yup, int nbins, bool convolve, bool swave, bool dwave)
{
/*Input***********************************************************************/
  TFile* file = TFile::Open(filename.c_str());
  TTree* tree = GetTree(file,cuts);
/*Fit the resolution**********************************************************/
  using namespace RooFit;
/*Do convolved fit************************************************************/
  RooRealVar* m = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
  cout << "Importing tree" << endl;
  RooDataSet* data;
  MassFitter* massfitter = new MassFitter(m);
  if(weight == "")
    data = new RooDataSet("data","",RooArgSet(*m),Import(*tree));
  else
  {
    RooRealVar* w = new RooRealVar(weight.c_str(),"weighting variable",-100,100);
    data = new RooDataSet("data","",RooArgSet(*m,*w),Import(*tree),WeightVar(*w));
    massfitter->SetWeighted();
  }
  RooRealVar* Nnonres  = new RooRealVar("N","Number of non-resonant candidates",1,0,2000);
  RooRealVar* Nphi     = new RooRealVar("N","Number of phi candidates",3000,0,10000);
  RooRealVar* Nftwop   = new RooRealVar("N","Number of f2'(1525) candidates",10,0,10000);
  
  Component* nonres;
  if(swave)
  {
    nonres = massfitter->AddComponent("nonres","Three Body Phase Space",Nnonres);
  }
  Component* phi;
  if(convolve)
  {
    phi = massfitter->AddComponent("phi","RBW(X)Gauss",Nphi);
  }
  else
  {
    phi = massfitter->AddComponent("phi","Rel Breit-Wigner",Nphi);
  }
  if(unit.find("GeV")!=string::npos)
  {
    phi->SetRange("mean",1.018,1.020);
    phi->FixValue("mean",1.019461);
    phi->SetRange("width",0.004,0.0045);
    phi->FixValue("width",0.004266);
    phi->SetRange("radius",1.5,5.0);
    phi->FixValue("radius",3);
    phi->SetRange("mK",0.493,0.494);
    phi->FixValue("mK",0.493677);
    if(convolve)
    {
      phi->SetRange("sigma1",0.0001,0.001);
      phi->SetValue("sigma1",0.0001);
    }
  }
  else
  {
    phi->SetRange("mean",1018,1020);
    phi->FixValue("mean",1019.461);
    phi->SetRange("width",4,4.5);
    phi->FixValue("width",4.266);
    phi->SetRange("radius",0.0015,0.005);
    phi->FixValue("radius",0.003);
    phi->SetRange("mK",493,494);
    phi->FixValue("mK",493.677);
    if(convolve)
    {
      phi->SetRange("sigma1",0.1,1.0);
      phi->SetValue("sigma1",0.1);
    }
  }
  Component* ftwop;
  if(dwave)
  {
    ftwop = massfitter->AddComponent("ftwop","Rel Breit-Wigner",Nftwop);
    ftwop->SetRange("mean",1520,1530);
    ftwop->FixValue("mean",1522.2);
    ftwop->SetRange("width",80,88);
    ftwop->FixValue("width",84);
    ftwop->SetRange("radius",1.5,5.0);
    ftwop->FixValue("radius",3);
    ftwop->FixValue("spin",2);
  }
  massfitter->Fit(data);
  RooPlot* frame = m->frame();
  cout << "Plotting" << endl;
  data->plotOn(frame,(Binning(nbins)));
  massfitter->Plot(frame);
  plotmaker plotter2(frame);
  RooHist* pullhist = frame->pullHist();
  RooPlot* pullframe = m->frame(Title("Pull"));
  pullframe->addPlotable(pullhist,"B");
  plotter2.SetPullPlot(pullframe);
  plotter2.SetTitle((xtitle), unit);
  if(yup>0)
    frame->SetMaximum(yup);
  frame->SetMinimum(0);
  TCanvas* can = plotter2.Draw();
  stringstream ytitle;
  ytitle << "#font[132]{}Candidates / (" << (xup-xlow)/nbins << " " << unit << ")";
  frame->GetYaxis()->SetTitle(ytitle.str().c_str());
  can->SaveAs((plotname+".pdf").c_str());
}
/*Main function***************************************************************/
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  string file, branch, cuts, xtitle, unit, plot, weight;
  double xlow, xup, yup;
  int nbins;
  desc.add_options()
    ("help,H"    ,                                                                                 "produce help message")
    ("conv"      ,                                                                                 "do convolution"      )
    ("swave"     ,                                                                                 "use s-wave component")
    ("dwave"     ,                                                                                 "use d-wave component")
    ("file,F"    , value<string>(&file  )->default_value("ntuples/BsphiKK_data_1800_mvacut.root"), "data ntuple"         )
    ("branch,B"  , value<string>(&branch)->default_value("KK_M"                                 ), "mass branch"         )
    ("cuts,C"    , value<string>(&cuts  )->default_value(""                                     ), "cuts"                )
    ("weight,W"  , value<string>(&weight)->default_value(""                                     ), "weight"              )
    ("title,T"   , value<string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}})"     ), "x-axis title"        )
    ("unit,U"    , value<string>(&unit  )->default_value("MeV/#it{c}^{2}"                       ), "x-axis unit"         )
    ("plot,O"    , value<string>(&plot  )->default_value("KKmassfit"                            ), "output plot filename")
    ("upper,u"   , value<double>(&xup   )->default_value(1800                                   ), "x-axis upper limit"  )
    ("lower,l"   , value<double>(&xlow  )->default_value(980                                    ), "x-axis lower limit"  )
    ("ymax"      , value<double>(&yup   )->default_value(-1                                     ), "y-axis upper limit"  )
    ("bins,b"    , value<int   >(&nbins )->default_value(41                                     ), "number of bins"      )
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
  mKKfit(file,branch,cuts,weight,xtitle,unit,plot,xlow,xup,yup,nbins,vmap.count("conv"),vmap.count("swave"),vmap.count("dwave"));
  return 0;
}
