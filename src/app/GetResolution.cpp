// Standard headers
#include <string>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
// RooFit headers
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooBreitWigner.h"
// Custom headers
#include "plotmaker.h"

void GetResolution(string filename, string particlename, string branchname, string xtitle, string unit, string plotname, double xlow, double xup, int nbins)
{
/*Input***********************************************************************/
  TFile* file = new TFile(filename.c_str());
  TTree* tree;
  if((TTree*)file->Get("DecayTreeTuple/DecayTree") != (TTree*)0x0)
  {
    tree  = (TTree*)file->Get("DecayTreeTuple/DecayTree");
  }
  else if((TTree*)file->Get("DecayTree") != (TTree*)0x0)
  {
    tree  = (TTree*)file->Get("DecayTree");
  }
  else if((TTree*)file->Get("MCDecayTree") != (TTree*)0x0)
  {
    tree  = (TTree*)file->Get("MCDecayTree");
  }
  else
  {
    cout << "Couldn't find tree." << endl;
    return;
  }
/*Construct resolution********************************************************/
  TTree* newtree = new TTree();
  double res; newtree->Branch("res",&res,"res/D");
  // KK_TRUEP branches are identical to B_s0_TRUEP (bug?)
  // phi_1020_TRUEP is fine, so are individual kaons
  TLorentzVector TRUEP[2];
  double TRUEP_E[2]; tree->SetBranchAddress("Kplus0_TRUEP_E",&TRUEP_E[0]); tree->SetBranchAddress("Kminus0_TRUEP_E",&TRUEP_E[1]);
  double TRUEP_X[2]; tree->SetBranchAddress("Kplus0_TRUEP_X",&TRUEP_X[0]); tree->SetBranchAddress("Kminus0_TRUEP_X",&TRUEP_X[1]);
  double TRUEP_Y[2]; tree->SetBranchAddress("Kplus0_TRUEP_Y",&TRUEP_Y[0]); tree->SetBranchAddress("Kminus0_TRUEP_Y",&TRUEP_Y[1]);
  double TRUEP_Z[2]; tree->SetBranchAddress("Kplus0_TRUEP_Z",&TRUEP_Z[0]); tree->SetBranchAddress("Kminus0_TRUEP_Z",&TRUEP_Z[1]);
  double SIMMASS; tree->SetBranchAddress((particlename+branchname).c_str(),&SIMMASS);
  for(int i = 0; i < tree->GetEntries(); i++)
  {
    tree->GetEntry(i);
    TRUEP[0].SetPxPyPzE(TRUEP_X[0],TRUEP_Y[0],TRUEP_Z[0],TRUEP_E[0]);
    TRUEP[1].SetPxPyPzE(TRUEP_X[1],TRUEP_Y[1],TRUEP_Z[1],TRUEP_E[1]);
    res = (TRUEP[0]+TRUEP[1]).M() - SIMMASS;
    newtree->Fill();
  }
/*Plot************************************************************************/
  using namespace RooFit;
  RooRealVar* x = new RooRealVar("res",xtitle.c_str(),xlow,xup);
  RooDataSet* data;
  std::cout << "Importing tree" << endl;
  data = new RooDataSet("data","",RooArgSet(*x),Import(*newtree));
  RooRealVar mean("mean","",0,-10,10);
  RooRealVar sigma("sigma","mass resolution",20,0,100);
  RooBreitWigner* model = new RooBreitWigner("model","",*x,mean,sigma);
  model->fitTo(*data);
  RooPlot* frame = x->frame();
  std::cout << "Plotting" << endl;
  data->plotOn(frame,Binning(nbins));
  model->plotOn(frame);
  frame->SetMaximum(frame->GetMaximum()*1.3);
  plotmaker plotter(frame);
  plotter.SetTitle(("#Delta"+xtitle), unit);
  plotter.Draw()->SaveAs((plotname+".pdf").c_str());
}
/*Main function***************************************************************/
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string file, partic, branch, cuts, xtitle, unit, plot, weight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"    ,                                                                                      "produce help message"                      )
    ("file,F"    , value<std::string>(&file  )->default_value("ntuples/BsphiKK_MC_mvacut.root"       ), "set data file"                             )
    ("particle,P", value<std::string>(&partic)->default_value("KK"                                   ), "set particle name"                         )
    ("branch,B"  , value<std::string>(&branch)->default_value("_M"                                   ), "set branch to compare to"                  )
    ("title,T"   , value<std::string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}})"     ), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"    , value<std::string>(&unit  )->default_value("MeV/#it{c}^{2}"                       ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"    , value<std::string>(&plot  )->default_value("massresolution"                       ), "set output plot filename"                  )
    ("upper,u"   , value<double     >(&xup   )->default_value(50                                     ), "set plot upper limit"                      )
    ("lower,l"   , value<double     >(&xlow  )->default_value(-50                                    ), "set plot lower limit"                      )
    ("bins,b"    , value<int        >(&nbins )->default_value(50                                     ), "set number of bins"                        )
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
  GetResolution(file,partic,branch,xtitle,unit,plot,xlow,xup,nbins);
  return 0;
}
