#include "Bs2PhiKKAcceptance.h"
#include "TMath.h"
#include "progbar.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include <string>
#include <iostream>
#include "GetTree.h"

using namespace std;
void testAngAcc(string filename)
{
  // double Bs2PhiKKAcceptance::Evaluate(double mKK, double phi, double ctheta_1, double ctheta_2)
//  double minima[4] = { 980,-TMath::Pi(),-1,-1};
//  double maxima[4] = {1800, TMath::Pi(), 1, 1};
  Bs2PhiKKAcceptance acc;
  TFile* file = new TFile(filename.c_str());
  TTree* tree = GetTree(file);
  double mKK;      tree->SetBranchAddress("KK_M"      ,&mKK     );
  double phi;      tree->SetBranchAddress("Phi_angle" ,&phi     );
  double ctheta_1; tree->SetBranchAddress("cos_theta1",&ctheta_1);
  double ctheta_2; tree->SetBranchAddress("cos_theta2",&ctheta_2);
  TH1D hist("hist","",100,-1,1);
  TH1D mKKh("mKKh","",100,Bs2PhiKKAcceptance::mKK_min,Bs2PhiKKAcceptance::mKK_max);
  TH1D phih("phih","",100,-TMath::Pi(),TMath::Pi());
  TH1D ct1h("ct1h","",100,-1,1);
  TH1D ct2h("ct2h","",100,-1,1);
  int n = tree->GetEntries();
  progbar bar(n);
  for(Int_t i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    if(mKK<Bs2PhiKKAcceptance::mKK_min||mKK>Bs2PhiKKAcceptance::mKK_max) continue;
    double acceptance = acc.Evaluate(mKK,phi,ctheta_1,ctheta_2)/0.04;
    hist.Fill(acceptance);
    if(i%100 == 0)
    {
      bar.print(i);
    }
    if(acceptance<0)
    {
      mKKh.Fill(mKK);
      phih.Fill(phi);
      ct1h.Fill(ctheta_1);
      ct2h.Fill(ctheta_2);
    }
  }
  bar.terminate();
//  hist.Draw();
//  gPad->SaveAs("hist.pdf");
  TCanvas canv;
  canv.Divide(2,2);
  canv.cd(1);
  mKKh.Draw();
  canv.cd(2);
  phih.Draw();
  canv.cd(3);
  ct1h.Draw();
  canv.cd(4);
  ct2h.Draw();
  canv.SaveAs("negativeacc.pdf");
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    cout << "Please provide a filename." << endl;
    return 1;
  }
  else if(argc>2)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  testAngAcc((string)argv[1]);
  return 0;
}
