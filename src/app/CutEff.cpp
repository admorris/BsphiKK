#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TH1.h"
#include "../mkplots.h"
#include <string>
#include <iostream>
#include <iomanip>
/******************************************************************************/
void cuteff(string filename = "LbphiKp_MC_mvaVars", string branchtoplot = "B_s0_M", string beforecut = "", string cut = "", string plotname = "")
{
//  cout << "Cut efficiency for \"" << cut <<"\" in " << filename << endl;
/*Input************************************************************************/
  // Open the input file
  TFile* infile  = new TFile(("../ntuples/"+filename+".root").c_str());
  // Get the input tree
  TTree* intree  = (TTree*)infile->Get("DecayTree");
  gStyle->SetOptStat(0);
  // Draw "before" and fetch the yield and the temporary histogram
  TCanvas* canbef = new TCanvas("before");
  canbef->cd();
  Long64_t nocutyield = intree->Draw(branchtoplot.c_str(),beforecut.c_str());
  TH1F* bef = (TH1F*)gPad->GetPrimitive("htemp");
  // Draw "after" and fetch yield and histo
  TCanvas* canaft = new TCanvas("after");
  canaft->cd();
  string aftercut;
  if(beforecut=="")
    aftercut=cut;
  else if(cut!="")
    aftercut=beforecut+"&&"+cut;
  else
    aftercut=beforecut;
  Long64_t cutyield   = intree->Draw(branchtoplot.c_str(),aftercut.c_str());
  TH1F* aft = (TH1F*)gPad->GetPrimitive("htemp");
  TH1F* dif = (TH1F*)bef->Clone();
  dif->Add(aft,-1);
  // Draw canvas
  TCanvas* can = new TCanvas((filename+" {"+cut+"}").c_str());
  TLegend* leg = new TLegend(0.6,0.5,0.89,0.65);
  can->cd();
  can->SetLeftMargin(0.12  );
  can->SetBottomMargin(0.12);
  can->SetFrameLineColor(0 );
  bef->Draw(       );
  aft->Draw("Esame");
  dif->Draw("Esame");
  bef->SetLineColor(kBlack);
  aft->SetLineColor(kBlue );
  dif->SetLineColor(kGreen);
  leg->SetHeader(filename.c_str());
  leg->AddEntry(bef, "Original", "l");
  leg->AddEntry(aft, "Passed",   "l");
  leg->AddEntry(dif, "Vetoed",   "l");
  leg->SetLineColor(0);
  leg->Draw();
  cout << "         Kept\tCut" << endl
       << "#events: " << cutyield << "\t" << nocutyield-cutyield << "\t" << endl
       << "percent: " << setprecision(3) << 100.0*cutyield/nocutyield << "%\t" << 100.0*(nocutyield-cutyield)/nocutyield << "%" << endl;
  if(plotname!="")
  {
    mkplots(can,plotname);
//    can->SaveAs(("./figs/"+plotname+".pdf").c_str());
//    can->SaveAs(("./figs/"+plotname+".eps").c_str());
//    can->SaveAs(("./figs/"+plotname+".png").c_str());
//    can->SaveAs(("./figs/"+plotname+".C").c_str());
  }
  return;
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    cout << "Usage: CutEff <filename> <branch to plot> <initial cut> <final cut> [<plot name>]" << endl;
    return 1;
  }
  else if(argc>5)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  cuteff((string)argv[1], (string)argv[2], (string)argv[3], (string)argv[4], (string)argv[5]);
  return 0;
}
