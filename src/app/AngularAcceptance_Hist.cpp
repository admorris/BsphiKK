#include <iostream>
#include <vector>


#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TKDTreeBinning.h"

#include "GetTree.h"
#include "FourDHist_Fixed.h"
#include "AngularAcceptanceFill.h"

using namespace std;
void AngularAcceptance(string selfile, string genfile)
{
  bool sym = true;
  TTree* tree = GetTree(genfile,"KK_M<1800");
  double x[4];
  FourDHist_Fixed genhist
  (
     4,sym? 0 : -TMath::Pi(), TMath::Pi()
    ,4,sym? 0 : -1, 1
    ,4,sym? 0 : -1, 1
    ,4,493*2,1800
  );
  genhist.SetAxisNames("phi","ctheta_1","ctheta_2","mKK");
  genhist.SetAxisTitles("#Phi","cos(#theta_{1})","cos(#theta_{2})","m(K^{#plus}K^{#minus})");
  Fill(x, tree, genhist, "KK_M", sym);
  tree = GetTree(selfile,"BCON_KK_M<1800&&abs(KK_TRUEID)>500");
  FourDHist_Fixed selhist = genhist;
  selhist.Clear();
  Fill(x, tree, selhist, "KK_M", sym);
  genhist.BinContentHist()->Draw();
  gPad->SaveAs("GenBinDist.pdf");
  selhist.BinContentHist()->Draw();
  gPad->SaveAs("SelBinDist.pdf");
  FourDHist_Fixed acchist = selhist / genhist;
  TFile output("AcceptanceProjections.root","RECREATE");
  TH1* h;
  gStyle->SetOptStat(0);
  for(int i = 0; i < 4; i++)
  {
    h = acchist.Project(i);
    h->SetMinimum(0);
    h->Draw();
    gPad->SaveAs(((string)h->GetName()+".pdf").c_str());
    h->Write();
    for(int j = i+1; j < 4; j++)
    {
      h = acchist.Project(i,j);
      h->SetMinimum(0);
      h->Draw("COLZ");
      gPad->SaveAs(((string)h->GetName()+".pdf").c_str());
      h->Write();
    }
  }
}
int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1], argv[2]);
  return 0;
}

