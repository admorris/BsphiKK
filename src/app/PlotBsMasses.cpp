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
using namespace std;
/******************************************************************************/
TH1D* plotBsmass(string filename = "LbphiKp_MC")
{
  cout << "Plotting Bs mass from " << filename << endl;
/*Input************************************************************************/
  // Open the input file
  TFile* infile  = new TFile(("../ntuples/"+filename+"_mvacut.root").c_str());
  // Get the input tree
  TTree* intree  = (TTree*)infile->Get("DecayTree");
  // Set branches to read
  double B_s0_LOKI_Mass; intree->SetBranchAddress("B_s0_LOKI_Mass",&B_s0_LOKI_Mass);
  double KK_M; intree->SetBranchAddress("KK_M",&KK_M);
/*Output***********************************************************************/
  TH1D* hist = new TH1D(filename.c_str(),"",100,5170,5580);
/*Fill*************************************************************************/
  for(int i = 0; i < intree->GetEntries(); i++)
  {
    intree->GetEntry(i);
    if(KK_M>1800) continue;
    hist->Fill(B_s0_LOKI_Mass);
  }
  return hist;
}
void plotBsmasses()
{
/*Input variables**************************************************************/
  // Generated events
  int ngen_LbphiKp   =  511582+ 540787;
  int ngen_BdphiKst  = 2007492+2020493;
  int ngen_Bsphipipi = 4004542+9016672;
  int ngen_Bsphiphi  = 1028748+1029997;
  int ngen_BsphiKK   = 1023767+1000157;
  // Fragmentation fraction ratios
  double fLb_fufd    = 0.404*(1-0.031*5); // 0.404(1-0.031*pT(GeV))
  double fs_fufd     = 0.134;
  double fs_fd       = 0.267;
  // Branching fractions
  double B_phitoKK   =   0.489;
  double B_KsttoKpi  = 2.0/3.0;
  double B_Bsphiphi  = 1.82e-5; // Get B_BsphiKK from B_BsJpsiKK, B_BsJpsiphi and B_Bsphiphi
  double B_BsJpsiphi = 1.05e-3;
  double B_BsJpsiKK  = 7.70e-4-B_phitoKK*B_BsJpsiphi; // Assumes
  double B_BsphiKK   = B_BsJpsiKK*B_Bsphiphi/B_BsJpsiphi;
  double B_LbJpsipK  = 3.04e-4; // LHCb-PAPER-2015-032 draft
  double B_LbphiKp   = B_LbJpsipK*B_Bsphiphi/B_BsJpsiphi;
  double B_Bsphipipi = 3.76e-6; // Haofei's result
  double B_BdphiKst  = 1.00e-5;
/*Get the histograms***********************************************************/
  TH1D* LbphiKp      = plotBsmass("LbphiKp_MC"  );
  TH1D* BdphiKst     = plotBsmass("BdphiKst_MC" );
  TH1D* Bsphipipi    = plotBsmass("Bsphipipi_MC");
  TH1D* Bsphiphi     = plotBsmass("Bsphiphi_MC" );
  TH1D* BsphiKK      = plotBsmass("BsphiKK_MC"  );
//  double Rdata = 2534/1961; // Ratio of yields from fits with mKK<1050 and 1050<mKK<1800
  TCanvas* can       = new TCanvas("can","");
/*Calculate normalisation for each mode****************************************/
  double denominator = B_Bsphiphi * B_phitoKK *(1.0/ngen_Bsphiphi);
  double w_BsphiKK   = 1961./2534. * Bsphiphi->GetEntries() / BsphiKK->GetEntries();//B_BsphiKK  *(1.0/ngen_BsphiKK) /denominator;
  double w_LbphiKp   = B_LbphiKp  *(1.0/ngen_LbphiKp)  /denominator;
  double w_BdphiKst  = B_BdphiKst *(1.0/ngen_BdphiKst) * B_KsttoKpi/denominator;
  double w_Bsphipipi = B_Bsphipipi*(1.0/ngen_Bsphipipi)/denominator;
  cout << "N_BsphiKK   = \t" << w_BsphiKK                   *BsphiKK->GetEntries()  /Bsphiphi->GetEntries() << "×N_Bsphiphi"  << endl
       << "N_LbphiKp   = \t" << w_LbphiKp*(fLb_fufd/fs_fufd)*LbphiKp->GetEntries()  /Bsphiphi->GetEntries() << "×N_Bsphiphi" << endl
       << "N_BdphiKst  = \t" << w_BdphiKst/fs_fd            *BdphiKst->GetEntries() /Bsphiphi->GetEntries() << "×N_Bsphiphi" << endl
       << "N_Bsphipipi = \t" << w_Bsphipipi                 *Bsphipipi->GetEntries()/Bsphiphi->GetEntries() << "×N_Bsphiphi" << endl;
  BsphiKK->Scale(w_BsphiKK);
  LbphiKp->Scale(w_LbphiKp*(fLb_fufd/fs_fufd));
  BdphiKst->Scale(w_BdphiKst/fs_fd);
  Bsphipipi->Scale(w_Bsphipipi);
/*Create stack*****************************************************************/
  THStack* total  = new THStack("total","");
  // Add components
  total->Add(Bsphipipi,"E");
  total->Add(LbphiKp,  "E");
  total->Add(BdphiKst, "E");
  total->Add(BsphiKK      );
  total->Add(Bsphiphi     );
  // Create legend
  TLegend* leg = new TLegend(0.6,0.5,0.89,0.8);
  leg->SetHeader("Decay mode");
  leg->AddEntry(BsphiKK,  "B^0_s\\to\\phi K^+ K^-",    "l");
  leg->AddEntry(BdphiKst, "B^0\\to\\phi K^*(892)",     "l");
  leg->AddEntry(LbphiKp,  "\\Lambda_b\\to\\phi p K^-", "l");
  leg->AddEntry(Bsphipipi,"B^0_s\\to\\phi\\pi^+\\pi^-","l");
  leg->SetLineColor(0);
  leg->SetTextSize(0.04);
/*Set colours******************************************************************/
  LbphiKp->SetLineColor(kViolet);
  BdphiKst->SetLineColor(kGreen);
  Bsphipipi->SetLineColor(kBlue);
  BsphiKK->SetLineColor(kBlack );
/*Style plot*******************************************************************/
  can->SetLeftMargin(0.12);
  can->SetBottomMargin(0.12);
  can->SetFrameLineColor(0);
  total->Draw();
  leg->Draw();
  stringstream ytitle;
  ytitle << "\\text{Candidates / }" << setprecision(1) << fixed << BsphiKK->GetBinWidth(0) << "~\\text{MeV}/c^2";
  total->GetYaxis()->SetTitle(ytitle.str().c_str());
  total->GetXaxis()->SetTitle("m_{K^+K^-K^+K^-}~\\text{MeV}/c^2");
  total->GetYaxis()->SetTitleSize(0.05);
  total->GetXaxis()->SetTitleSize(0.05);
/*Draw plot********************************************************************/
  can->SaveAs("simulated_backgrounds.pdf");
  can->SaveAs("simulated_backgrounds.eps");
  can->SaveAs("simulated_backgrounds.png");
  return;
}
int main()
{
  plotBsmasses();
  return 0;
}
