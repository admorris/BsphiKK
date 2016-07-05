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
#include "FourDHist.h"

using namespace std;
// End of class definition. Now to do something with it.
void Fill(double* x, TTree* tree, FourDHist& hist, bool sym)
{
  int n = tree->GetEntries();
  cout << "Tree contains " << n << " entries" << endl;
  tree->SetBranchAddress("Phi_angle" ,&x[0]);
  tree->SetBranchAddress("cos_theta1",&x[1]);
  tree->SetBranchAddress("cos_theta2",&x[2]);
  tree->SetBranchAddress("KK_M"      ,&x[3]);
  cout << "Entering event loop" << endl;
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    sym ?
      hist.Fill(TMath::Abs(x[0]),TMath::Abs(x[1]),TMath::Abs(x[2]),TMath::Abs(x[3]))
      :
      hist.Fill(x[0],x[1],x[2],x[3]);
  }
  cout << "The emptiest bin contains " << hist.MinBinContent() << " events" << endl;
}
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
  double target_error = 0.1;
  int nbins = n*pow(target_error,2);
  cout << "Using " << nbins << " bins." << endl;
  TKDTreeBinning magicbinningthing(n,4,data,nbins);
  double** bins = new double*[4];
  for(int idim = 0; idim < 4; idim++)
  {
    bins[idim] = new double[n];//?
  }
  // End adaptive binning stuff
  FourDHist selhist
  (
     4,sym? 0 : -TMath::Pi(), TMath::Pi()
    ,4,sym? 0 : -1, 1
    ,4,sym? 0 : -1, 1
    ,4,493*2,1800
  );
//  FourDHist selhist
//  (
//    
//  )
  selhist.SetAxisNames("phi","ctheta_1","ctheta_2","mKK");
  selhist.SetAxisTitles("#Phi","cos(#theta_{1})","cos(#theta_{2})","m(K^{#plus}K^{#minus})");
  Fill(x, tree, selhist, sym);
  tree = GetTree(genfile);
  FourDHist genhist = selhist;
  genhist.Clear();
  Fill(x, tree, genhist, sym);
  selhist.BinContentHist()->Draw();
  gPad->SaveAs("SelBinDist.pdf");
  genhist.BinContentHist()->Draw();
  gPad->SaveAs("GenBinDist.pdf");
  FourDHist acchist = selhist / genhist;
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
  cout << "End of main()" << endl;
  return 0;
}

