#ifndef __AngularAcceptanceFill_h__
#define __AngularAcceptanceFill_h__
#include <iostream>
#include "NDHist.h"
#include "TTree.h"
#include "progbar.h"
using namespace std;
void Fill(vector<string> branches, TTree* tree, NDHist& hist, bool sym = false)
{
  int n = tree->GetEntries();
  cout << "Tree contains " << n << " entries" << endl;
  vector<double> x; // This is the datapoint
  const int ndims = branches.size();
  for(int i = ndims; i-->0;)
    x.push_back(0);
  for(auto& branch : branches)
  {
    cout << "Reading branch " << branch << endl;
    tree->SetBranchAddress(branch.c_str(), &x[&branch - &branches[0]]);
  }
  cout << "Entering event loop" << endl;
  progbar bar(n);
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    if(sym) // Modulus of each component in the datapoint
      for(auto& val : x)
        val = abs(val);
    hist.Fill(x);
    if(i%1000==0) bar.print(i);
  }
  bar.terminate();
  // Print some stats about the histogram
  cout << "The fullest bin contains " << hist.MaxBinContent() << " events." << endl;
  cout << "The emptiest bin contains " << hist.MinBinContent() << " events." << endl;
  cout << "There are " << hist.OverFlow() << " overflow and " << hist.UnderFlow() << " underflow events." << endl;
  if(hist.Integral() != n)
    cout << "The histogram contains " << hist.Integral() << " events" << endl;
}
#endif
