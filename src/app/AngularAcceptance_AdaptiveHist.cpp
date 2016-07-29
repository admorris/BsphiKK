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
#include "FourDHist_Adaptive.h"
#include "AngularAcceptanceFill.h"

using namespace std;
void AngularAcceptance(string selfile, string genfile)
{
  bool sym = true;
  TTree* gentree = GetTree(genfile/*,"KK_M<1800"*/);
  TTree* seltree = GetTree(selfile,"KK_M<1800");
  double x[4];
  // Begin adaptive binning stuff
  const int n = gentree->GetEntries();
  cout << "Attempting to make an array with " << n*4 << " elements" << endl;
  vector<double> data; // To use the TKDTreeBinning constructor with a vector rather than a double, you need ROOT v6.07 ........ but that version isn't linked against GSL properly because why should anything work propely at this multi-billion-pound facility?
  for(int i = 0; i < n*4; i++)
    data.push_back(0); //lol
  gentree->SetBranchAddress("Phi_angle" ,&x[0]);
  gentree->SetBranchAddress("cos_theta1",&x[1]);
  gentree->SetBranchAddress("cos_theta2",&x[2]);
  gentree->SetBranchAddress("KK_M"      ,&x[3]);
  for(int i = 0; i < n; i++)
  {
    gentree->GetEntry(i);
    x[3]/=1000; // MeV to GeV
//    if(i%10000==0) cout << i << endl;
    for(int j = 0; j < 4; j++)
      data[n*j+i] = sym ? TMath::Abs(x[j]) : x[j];
  }
  int nbins = 232;
  cout << "Using " << nbins << " bins." << endl;
  TKDTreeBinning binner(n,4,&data[0],nbins);
  // End adaptive binning stuff
  FourDHist_Adaptive selhist(&binner);
  Fill(x, seltree, selhist, "BCON_KK_M", sym);
  selhist.BinContentHist()->Draw();
  gPad->SaveAs("SelBinDist.pdf");
  FourDHist_Adaptive genhist = selhist;
  genhist.Clear();
  Fill(x, gentree, genhist, "KK_M", sym);
  genhist.BinContentHist()->Draw();
  gPad->SaveAs("GenBinDist.pdf");
  TFile output("Acceptance.root","RECREATE");
  FourDHist_Adaptive acchist = selhist / genhist;
  acchist.SaveToTree()->Write();
  binner.GetTree()->Write();
  output.Close();
}
int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1], argv[2]);
  return 0;
}

