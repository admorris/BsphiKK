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
// Custom headers
#include "plotmaker.h"

void GetResolution(string filename, string particlename, string branchname, string xtitle, string unit, string plotname, double xlow, double xup, int nbins)
{
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
  TTree* newtree = new TTree();
  double res; tree->Branch("res",&res,"res/D");
  TLorentzVector TRUEP;
  double TRUEP_E; tree->SetBranchAddress((particlename+"_TRUEP_E").c_str(),&TRUEP_E);
  double TRUEP_X; tree->SetBranchAddress((particlename+"_TRUEP_X").c_str(),&TRUEP_X);
  double TRUEP_Y; tree->SetBranchAddress((particlename+"_TRUEP_Y").c_str(),&TRUEP_Y);
  double TRUEP_Z; tree->SetBranchAddress((particlename+"_TRUEP_Z").c_str(),&TRUEP_Z);
  double SIMMASS; tree->SetBranchAddress(branchname.c_str(),&SIMMASS);
  for(int i = 0; i < tree->GetEntries(); i++)
  {
    tree->GetEntry(i);
    TRUEP.SetPxPyPzE(TRUEP_X,TRUEP_Y,TRUEP_Z,TRUEP_E);
    res = TRUEP.M() - SIMMASS;
    newtree->Fill();
  }
  
  
  
  using namespace RooFit;
  RooRealVar* x = new RooRealVar("res",xtitle.c_str(),xlow,xup);
  RooDataSet* data;
  std::cout << "Importing tree" << endl;
  data = new RooDataSet("data","",RooArgSet(*x),Import(*newtree));
  RooPlot* frame = x->frame();
  std::cout << "Plotting" << endl;
  data->plotOn(frame,Binning(nbins));
  frame->SetMaximum(frame->GetMaximum()*1.3);
  plotmaker plotter(frame);
  plotter.SetTitle(xtitle, unit);
  plotter.Draw()->SaveAs((plotname+".pdf").c_str());
}

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
    ("branch,B"  , value<std::string>(&branch)->default_value("_LOKI_Mass"                           ), "set branch to compare to"                  )
    ("title,T"   , value<std::string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}})"     ), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"    , value<std::string>(&unit  )->default_value("MeV/#it{c}^{2}"                       ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"    , value<std::string>(&plot  )->default_value("massresolution"                       ), "set output plot filename"                  )
    ("upper,u"   , value<double     >(&xup   )->default_value(10                                     ), "set plot upper limit"                      )
    ("lower,l"   , value<double     >(&xlow  )->default_value(-10                                    ), "set plot lower limit"                      )
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
