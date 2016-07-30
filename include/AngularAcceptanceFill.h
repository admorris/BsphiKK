#ifndef __AngularAcceptanceFill_h__
#define __AngularAcceptanceFill_h__
#include <iostream>
#include "FourDHist.h"
#include "TTree.h"
using namespace std;
void Fill(double* x, TTree* tree, FourDHist& hist, const char* mKKbranch = "KK_M", bool sym = false)
{
  int n = tree->GetEntries();
  cout << "Tree contains " << n << " entries" << endl;
  tree->SetBranchAddress("Phi_angle" ,&x[0]);
  tree->SetBranchAddress("cos_theta1",&x[1]);
  tree->SetBranchAddress("cos_theta2",&x[2]);
  tree->SetBranchAddress(mKKbranch   ,&x[3]); // option to use BCON branch
  cout << "Entering event loop" << endl;
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    x[3] /= 1000; // MeV to GeV
    sym ?
      hist.Fill(TMath::Abs(x[0]),TMath::Abs(x[1]),TMath::Abs(x[2]),TMath::Abs(x[3]))
      :
      hist.Fill(x[0],x[1],x[2],x[3]);
  }
  cout << "The fullest bin contains " << hist.MaxBinContent() << " events." << endl;
  cout << "The emptiest bin contains " << hist.MinBinContent() << " events." << endl;
  cout << "There are " << hist.OverFlow() << " overflow and " << hist.UnderFlow() << " underflow events." << endl;
  if(hist.Integral() != n)
    cout << "The histogram contains " << hist.Integral() << " events" << endl;
}
#endif
