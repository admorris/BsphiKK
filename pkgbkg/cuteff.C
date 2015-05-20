#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include <string>
#include <iostream>
#include <iomanip>
/******************************************************************************/
void cuteff(string filename = "LbphiKp_MC_bdtVars", string branchtoplot = "B_s0_M", string beforecut="", string cut = "")
{
//  cout << "Cut efficiency for \"" << cut <<"\" in " << filename << endl;
/*Input************************************************************************/
  // Open the input file
  TFile* infile  = new TFile(("../ntuples/"+filename+".root").c_str());
  // Get the input tree
  TTree* intree  = (TTree*)infile->Get("DecayTree");
  new TCanvas((filename+" {"+cut+"}").c_str());
  gStyle->SetOptStat(0);
  Long64_t nocutyield = intree->Draw(branchtoplot.c_str(),beforecut.c_str());
  string aftercut;
  if(beforecut=="")
    aftercut=cut;
  else
    aftercut=beforecut+"&&"+cut;
  Long64_t cutyield   = intree->Draw(branchtoplot.c_str(),aftercut.c_str(),"same");
  cout << "         Kept\tCut" << endl
       << "#events: " << cutyield << "\t" << nocutyield-cutyield << "\t" << endl
       << "percent: " << setprecision(3) << 100.0*cutyield/nocutyield << "%\t" << 100.0*(nocutyield-cutyield)/nocutyield << "%" << endl;
  return;
}
