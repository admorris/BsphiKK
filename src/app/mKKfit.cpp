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
void mKKfit(string filename, string branchname, string cuts, string weight, string xtitle, string unit, string plotname, double xlow, double xup, double yup, int nbins, bool convolve)
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
//  RooRealVar* Nnonres  = new RooRealVar("N","Number of non-resonant candidates",1,0,2);
  RooRealVar* Nphi     = new RooRealVar("N","Number of phi candidates",3000,0,10000);
//  RooRealVar* Nftwop   = new RooRealVar("N","Number of f2'(1525) candidates",10,0,100);
  
//  Component* nonres = massfitter->AddComponent("nonres","Flat",Nnonres);
  Component* phi;
  if(convolve)
  {
//  phi = massfitter->AddComponent("phi","Voigtian",Nphi);
//  phi = massfitter->AddComponent("phi","BW(X)Gauss",Nphi);
  phi = massfitter->AddComponent("phi","RBW(X)Gauss",Nphi);
  }
  else
  {
//  phi = massfitter->AddComponent("phi","Breit-Wigner",Nphi);
  phi = massfitter->AddComponent("phi","Rel Breit-Wigner",Nphi);
  }
  phi->FixValue("mean",1019.461);
  phi->FixValue("width",4.266);
//  phi->FixValue("sigma1",1.0);
//  phi->FixValue("spin",1);
//  phi->FixValue("radius",1.5);
/*  Component* ftwop = massfitter->AddComponent("ftwop","Breit-Wigner",Nftwop);
//  Component* ftwop = massfitter->AddComponent("ftwop","Rel Breit-Wigner",Nftwop);
  ftwop->SetRange("mean",1520,1530);
  ftwop->FixValue("mean",1522.2);
  ftwop->SetRange("width",80,88);
  ftwop->FixValue("width",84);
  //ftwop->FixValue("spin",2);*/
  massfitter->UsePhaseSpace(5366.77, 493.677, 493.677, 1019.461);
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
    ("help,H"    ,                                                                                        "produce help message")
    ("conv"      ,                                                                                        "do convolution"      )
    ("file,F"    , value<string>(&file  )->default_value("ntuples/BsphiKK_data_1800_mvacut.root"       ), "data ntuple"         )
    ("branch,B"  , value<string>(&branch)->default_value("KK_M"                                        ), "mass branch"         )
    ("cuts,C"    , value<string>(&cuts  )->default_value(""                                            ), "cuts"                )
    ("weight,W"  , value<string>(&weight)->default_value(""                                            ), "weight"              )
    ("title,T"   , value<string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}})"            ), "x-axis title"        )
    ("unit,U"    , value<string>(&unit  )->default_value("MeV/#it{c}^{2}"                              ), "x-axis unit"         )
    ("plot,O"    , value<string>(&plot  )->default_value("KKmassfit"                                   ), "output plot filename")
    ("upper,u"   , value<double>(&xup   )->default_value(1800                                          ), "x-axis upper limit"  )
    ("lower,l"   , value<double>(&xlow  )->default_value(980                                           ), "x-axis lower limit"  )
    ("ymax"      , value<double>(&yup   )->default_value(-1                                            ), "y-axis upper limit"  )
    ("bins,b"    , value<int   >(&nbins )->default_value(41                                            ), "number of bins"      )
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
  mKKfit(file,branch,cuts,weight,xtitle,unit,plot,xlow,xup,yup,nbins,vmap.count("conv"));
  return 0;
}
