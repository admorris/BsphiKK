#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TLegend.h"
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
  TH1D* hist = new TH1D(filename.c_str(),"",40,5170,5580);
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
/*Input variables**************************************************************/
  // Generated events
  Int_t ngen_LbphiKp   =  511582+540787;
  Int_t ngen_BdphiKst  = 2007492+2020493;
  Int_t ngen_Bsphipipi = 4004542+9016672;
  Int_t ngen_Bsphiphi  = 1028748+1029997;
  // Fragmentation fraction ratios
  Double_t fLb_fufd = 0.404*(1-0.031*10); // 0.404(1-0.031*pT(GeV))
  Double_t fs_fufd  = 0.134;
  Double_t fs_fd    = 0.267;
  // Branching fractions
  Double_t B_Bsphiphi  = 1.82e-5;
  Double_t B_BsJpsiphi = 1.09e-3;
  Double_t B_LbJpsipK  = 1.0e-4; // complete guess
  Double_t B_LbphiKp   = B_LbJpsipK*B_Bsphiphi/B_BsJpsiphi;
  Double_t B_Bsphipipi = 5.0e-6; // complete guess
  Double_t B_BdphiKst  = 1.00e-5;
  Double_t B_phitoKK   = 0.489;
  Double_t B_KsttoKpi  = 2.0/3.0;
/*Get the histograms***********************************************************/
  TH1D* LbphiKp   = plotBsmass("LbphiKp_MC"  );
  TH1D* BdphiKst  = plotBsmass("BdphiKst_MC" );
  TH1D* Bsphipipi = plotBsmass("Bsphipipi_MC");
  TH1D* Bsphiphi  = plotBsmass("Bsphiphi_MC" );
  TCanvas* can    = new TCanvas("can","");
/*Calculate normalisation for each mode****************************************/
  Double_t denominator = B_Bsphiphi *(1.0/ngen_Bsphiphi) *B_phitoKK;
  Double_t w_LbphiKp   = B_LbphiKp  *(1.0/ngen_LbphiKp)  /denominator;
  Double_t w_BdphiKst  = B_BdphiKst *(1.0/ngen_BdphiKst) *B_KsttoKpi/denominator;
  Double_t w_Bsphipipi = B_Bsphipipi*(1.0/ngen_Bsphipipi)/denominator;
  Bsphiphi->Scale(1.0); // Probably don't need this line
  LbphiKp->Scale(w_LbphiKp*(fLb_fufd/fs_fufd));
  BdphiKst->Scale(w_BdphiKst/fs_fd);
  Bsphipipi->Scale(w_Bsphipipi);
/*Create stack*****************************************************************/
  THStack* total  = new THStack("total","");
  // Add components
  total->Add(Bsphipipi,"E");
  total->Add(LbphiKp,  "E");
  total->Add(BdphiKst, "E");
  total->Add(Bsphiphi     );
  // Create legend
  TLegend* leg = new TLegend(0.6,0.5,0.89,0.8);
  leg->SetHeader("Decay mode");
  leg->AddEntry(Bsphiphi, "B^0_s\\to\\phi\\phi",       "l");
  leg->AddEntry(BdphiKst, "B^0\\to\\phi K^*(892)",     "l");
  leg->AddEntry(LbphiKp,  "\\Lambda_b\\to\\phi p K^-", "l");
  leg->AddEntry(Bsphipipi,"B^0_s\\to\\phi\\pi^+\\pi^-","l");
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);
/*Set colours******************************************************************/
  // Line colours chosen by length of word in order to get all the following lines the same length
  LbphiKp->SetLineColor(kViolet);
  BdphiKst->SetLineColor(kGreen);
  Bsphipipi->SetLineColor(kBlue);
  Bsphiphi->SetLineColor(kBlack);
  // Seriously look at how good that looks
/*Style plot*******************************************************************/
  can->SetLeftMargin(0.12);
  can->SetBottomMargin(0.12);
  can->SetFrameLineColor(0);
  total->Draw();
  leg->Draw();
  stringstream ytitle;
  ytitle << "\\text{Candidates / }" << setprecision(1) << fixed << Bsphiphi->GetBinWidth(0) << "~\\text{MeV}/c^2";
  total->GetYaxis()->SetTitle(ytitle.str().c_str());
  total->GetXaxis()->SetTitle("m_{K^+K^-K^+K^-}~\\text{MeV}/c^2");
  total->GetYaxis()->SetTitleSize(0.05);
  total->GetXaxis()->SetTitleSize(0.05);
/*Draw plot********************************************************************/
  mkplots(can,"simulated_backgrounds");
  return;
}
