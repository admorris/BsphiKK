#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include <string>
#include <iostream>
#include <iomanip>
/******************************************************************************/
void cuteff(string filename = "LbphiKp_MC", string branchtoplot = "B_s0_M", string beforecut="mlp>0.88&&B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5500", string cut = "Kminus_ProbNNpi<0.01")
{
  cout << "Cut efficiency for \"" << cut <<"\" in " << filename << endl;
/*Input************************************************************************/
  // Open the input file
  TFile* infile  = new TFile(("../ntuples/"+filename+"_bdt.root").c_str());
  // Get the input tree
  TTree* intree  = (TTree*)infile->Get("DecayTree");
  new TCanvas((filename+" {"+cut+"}").c_str());
  gStyle->SetOptStat(0);
  Long64_t nocutyield = intree->Draw(branchtoplot.c_str(),beforecut.c_str());
  Long64_t cutyield   = intree->Draw(branchtoplot.c_str(),(beforecut+"&&"+cut).c_str(),"same");
  cout << setprecision(2) << 100.0*cutyield/nocutyield << "%" << endl;
  return;
}
