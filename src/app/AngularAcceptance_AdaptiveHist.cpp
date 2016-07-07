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
  TTree* tree = GetTree(selfile,"KK_M<1800");
  double x[4];
  // Begin adaptive binning stuff
  const int n = tree->GetEntries();
  double data[n*4];
  tree->SetBranchAddress("Phi_angle" ,&x[0]);
  tree->SetBranchAddress("cos_theta1",&x[1]);
  tree->SetBranchAddress("cos_theta2",&x[2]);
  tree->SetBranchAddress("KK_M"      ,&x[3]);
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    for(int j = 0; j < 4; j++)
      data[n*j+i] = sym ? TMath::Abs(x[j]) : x[j];
  }
  int nbins = n/25;
  cout << "Using " << nbins << " bins." << endl;
  TKDTreeBinning binner(n,4,data,nbins);
  double** bins = new double*[4];
  for(int idim = 0; idim < 4; idim++)
  {
    bins[idim] = new double[n];//?
  }
  // End adaptive binning stuff
  FourDHist_Adaptive selhist(&binner);
  Fill(x, tree, selhist, sym);
  selhist.BinContentHist()->Draw();
  gPad->SaveAs("SelBinDist.pdf");
  tree = GetTree(genfile,"KK_M<1800");
  FourDHist_Adaptive genhist = selhist;
  genhist.Clear();
  Fill(x, tree, genhist, sym);
  genhist.BinContentHist()->Draw();
  gPad->SaveAs("GenBinDist.pdf");
  TFile output("Acceptance.root","RECREATE");
  FourDHist_Adaptive acchist = selhist / genhist;
  acchist.SaveToTree()->Write();
  binner.Write();
  output.Close();
}
int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1], argv[2]);
  return 0;
}

