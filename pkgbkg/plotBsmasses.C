#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "THStack.h"
#include "TStyle.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "../mkplots.h"
/******************************************************************************/
TH1D* plotBsmass(string filename = "LbphiKp_MC")
{
  cout << "Plotting Bs mass from " << filename << endl;
/*Input************************************************************************/
  // Open the input file
  TFile* infile  = new TFile(("../ntuples/"+filename+"_cuts.root").c_str());
  // Get the input tree
  TTree* intree  = (TTree*)infile->Get("DecayTree");
  // Set branches to read
  Double_t B_s0_LOKI_Mass;
  intree->SetBranchAddress("B_s0_LOKI_Mass",&B_s0_LOKI_Mass);
/*Output***********************************************************************/
  TH1D* hist = new TH1D(filename.c_str(),"",40,5366.77-200,5366.77+200);
/*Fill*************************************************************************/
  for(Int_t i = 0; i < intree->GetEntries(); i++)
  {
    intree->GetEntry(i);
    hist->Fill(B_s0_LOKI_Mass);
  }
  return hist;
}
void plotBsmasses()
{
/*Get the histograms***********************************************************/
  TH1D* LbphiKp   = plotBsmass("LbphiKp_MC"  );
  TH1D* BdphiKst  = plotBsmass("BdphiKst_MC" );
  TH1D* Bsphipipi = plotBsmass("Bsphipipi_MC");
  TH1D* BsphiKK   = plotBsmass("BsphiKK_MC"  );
  TCanvas* can    = new TCanvas("can","");
/*Create stack*****************************************************************/
  THStack* total  = new THStack("total","");
  total->Add(LbphiKp);
  total->Add(BdphiKst);
  total->Add(Bsphipipi);
  total->Add(BsphiKK);
/*Set colours******************************************************************/
  LbphiKp->SetLineColor(kRed);
  BdphiKst->SetLineColor(kGreen);
  Bsphipipi->SetLineColor(kBlue);
  BsphiKK->SetLineColor(kBlack);
/*Style plot*******************************************************************/
  can->SetLeftMargin(0.12);
  can->SetBottomMargin(0.12);
  total->Draw("E");
  stringstream ytitle;
  ytitle << "\\text{Candidates / }" << setprecision(1) << fixed << BsphiKK->GetBinWidth(0) << "~\\text{MeV}/c^2";
  total->GetYaxis()->SetTitle(ytitle.str().c_str());
  total->GetXaxis()->SetTitle("m_{K^+K^-K^+K^-}~\\text{MeV}/c^2");
  total->GetYaxis()->SetTitleSize(0.05);
  total->GetXaxis()->SetTitleSize(0.05);
/*Draw plot********************************************************************/
  mkplots(can,"simulated_backgrounds");
  return;
}
