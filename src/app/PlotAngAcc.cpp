#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TGraph.h"
#include <string>
#include <iostream>

using namespace std;
void PlotAngAcc(string filename,string plotfilename)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("tuple");
  TFile* plotfile = new TFile(plotfilename.c_str() );
  TCanvas* can = (TCanvas*)plotfile->Get("acc_can" );
  TPad* pad = (TPad*)can->GetPrimitive("acc_can_1" );
  TH1D* mKKAccData = (TH1D*)pad->GetPrimitive("mKKAcc"    );
  TH1D* mKKAccProj = (TH1D*)pad->GetPrimitive("mKKAccProj");
  pad       = (TPad*)can->GetPrimitive("acc_can_2" );
  TH1D* phiAccData = (TH1D*)pad->GetPrimitive("phiAcc"    );
  TH1D* phiAccProj = (TH1D*)pad->GetPrimitive("phiAccProj");
  pad       = (TPad*)can->GetPrimitive("acc_can_3" );
  TH1D* cosThetaAccData = (TH1D*)pad->GetPrimitive("cosThetaAcc"    );
  TH1D* cosThetaAccProj = (TH1D*)pad->GetPrimitive("cosThetaAccProj");
  pad       = (TPad*)can->GetPrimitive("acc_can_4" );
  TH1D* cosPsiAccData = (TH1D*)pad->GetPrimitive("cosPsiAcc"    );
  TH1D* cosPsiAccProj = (TH1D*)pad->GetPrimitive("cosPsiAccProj");
  
  gStyle->SetOptStat(0);
  TCanvas* canvmKK = new TCanvas();
  mKKAccData->SetMinimum(0);
  mKKAccData->SetTitle("");
  mKKAccData->GetXaxis()->SetTitle("#it{m}(#it{K}^{#plus}#it{K}^{#minus}) [MeV/#it{c}^{2}]");
  mKKAccData->Draw();
  mKKAccProj->Draw("same");
  canvmKK->SaveAs("mKK_proj.pdf");
  TCanvas* canvphi = new TCanvas();
  phiAccData->SetMinimum(0);
  phiAccData->SetTitle("");
  phiAccData->GetXaxis()->SetTitle("#it{#Phi}");
  phiAccData->Draw();
  phiAccProj->Draw("same");
  canvphi->SaveAs("phi_proj.pdf");
  TCanvas* canvcosTheta = new TCanvas();
  cosThetaAccData->SetMinimum(0);
  cosThetaAccData->SetTitle("");
  cosThetaAccData->GetXaxis()->SetTitle("cos(#it{#theta_{1}})");
  cosThetaAccData->Draw();
  cosThetaAccProj->Draw("same");
  canvcosTheta->SaveAs("ctheta_1_proj.pdf");
  TCanvas* canvcosPsi = new TCanvas();
  cosPsiAccData->SetMinimum(0);
  cosPsiAccData->SetTitle("");
  cosPsiAccData->GetXaxis()->SetTitle("cos(#it{#theta_{2}})");
  cosPsiAccData->Draw();
  cosPsiAccProj->Draw("same");
  canvcosPsi->SaveAs("ctheta_2_proj.pdf");
  
  TCanvas canv[6];
  string comb[6] = {"ctheta_1:ctheta_2","ctheta_1:phi","ctheta_2:phi","phi:mKK","ctheta_1:mKK","ctheta_2:mKK"};
  for(int i = 0; i < 6; i++)
  {
    canv[i].Draw();
    canv[i].cd();
    tree->Draw(comb[i].c_str(),"weight","COLZ");
    TH2F* graph = (TH2F*)canv[i].GetPrimitive("htemp");
    graph->SetTitle("");
    cout << "Fetched graph" << endl;
    std::replace(comb[i].begin(),comb[i].end(), ':', '-');
    canv[i].SaveAs((comb[i]+".pdf").c_str());
  }
  
  
//  tree->Draw();
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    PlotAngAcc("sampled_acceptance.root","acceptance.root");
    return 0;
  }
  else if(argc>3)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  PlotAngAcc((string)argv[1],(string)argv[2]);
  return 0;
}
