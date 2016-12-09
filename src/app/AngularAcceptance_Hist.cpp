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
#include "NDHist_Fixed.h"
#include "AngularAcceptanceFill.h"

using namespace std;
void AngularAcceptance(string selfile, string genfile)
{
  bool sym = true;
  // Set up the denominator histogram
  vector<double> massedges = {980,1050,1400,1800};
  vector<double> costhetaedges = {0,0.5,0.75,1};
  vector<double> phiedges = {0, TMath::Pi()/2, TMath::Pi()};
  NDHist_Fixed genhist
  ({
     std::tuple<int,double*>(massedges.size()-1,&massedges[0])
    ,std::tuple<int,double*>(costhetaedges.size()-1,&costhetaedges[0])
    ,std::tuple<int,double*>(costhetaedges.size()-1,&costhetaedges[0])
    ,std::tuple<int,double*>(phiedges.size()-1,&phiedges[0])
  });
  genhist.SetAxisNames({"mKK","ctheta_1","ctheta_2","phi"});
  genhist.SetAxisTitles({"m(K^{#plus}K^{#minus})","cos(#theta_{1})","cos(#theta_{2})","#Phi"});
  NDHist_Fixed selhist = genhist;
  vector<string> branches = { "KK_M"
                            , "cos_theta1"
                            , "cos_theta2"
                            , "Phi_angle"
                            };
  // Fill the denominator histogram
//  TTree* gentree = GetTree(genfile,"KK_M<1800");
  TTree* gentree = GetTree(genfile);
  Fill(branches, gentree, genhist, sym);
  for(auto& branch : branches) branch = "BCON_"+branch;
//  TTree* seltree = GetTree(selfile,"BCON_KK_M<1800&&abs(KK_TRUEID)>500");
  TTree* seltree = GetTree(selfile);
  Fill(branches, seltree, selhist, sym);
  // Write some output
  TFile output("Acceptance.root","RECREATE");
  genhist.BinContentHist()->Draw();
  gPad->SaveAs("GenBinDist.pdf");
  selhist.BinContentHist()->Draw();
  gPad->SaveAs("SelBinDist.pdf");
  NDHist_Fixed acchist = selhist / genhist;
//  acchist.Write();
  output.Close();
  /*
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
  */
}
int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1], argv[2]);
  return 0;
}

