#ifndef __AngularAcceptanceFill_h__
#define __AngularAcceptanceFill_h__
#include <iostream>
#include "NDHist.h"
#include "TTree.h"
using namespace std;
void Fill(vector<string> branches, TTree* tree, NDHist& hist, bool sym = false)
{
  int n = tree->GetEntries();
  cout << "Tree contains " << n << " entries" << endl;
  vector<double> x;
  for(auto branch : branches)
  {
    x.push_back(0);
    tree->SetBranchAddress(branch.c_str(), &x.back());
  }
  cout << "Entering event loop" << endl;
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    x[0] /= 1000; // MeV to GeV
    if(sym)
      for(auto& val : x)
        val = abs(val);
    hist.Fill(x);
  }
  cout << "The fullest bin contains " << hist.MaxBinContent() << " events." << endl;
  cout << "The emptiest bin contains " << hist.MinBinContent() << " events." << endl;
  cout << "There are " << hist.OverFlow() << " overflow and " << hist.UnderFlow() << " underflow events." << endl;
  if(hist.Integral() != n)
    cout << "The histogram contains " << hist.Integral() << " events" << endl;
}
#endif
