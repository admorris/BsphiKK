#include "FourDHist_Adaptive.h"
#include "TCollection.h"
#include "TKey.h"
#include <stdexcept>
#include <assert.h>
#include <iostream>
using namespace std;
FourDHist_Adaptive::FourDHist_Adaptive(TKDTreeBinning* _b) : binner(_b->GetTree())
{
  Initialise();
}
FourDHist_Adaptive::FourDHist_Adaptive(TKDTreeID* _b) : binner(_b)
{
  Initialise();
}
FourDHist_Adaptive::FourDHist_Adaptive(TFile* _f)
{
  TIter next = _f->GetListOfKeys();
  TKey* key;
  // Look for any TKDTree object in the file
  while((key = (TKey*)next()))
  {
    cout << key->GetClassName() << endl;
    if(strcmp(key->GetClassName(),"TKDTree<int,double>")==0)
    {
      binner = (TKDTreeID*)key->ReadObj();
      Initialise();
    }
  }
  if(binner == (TKDTreeID*)0x0)
    throw runtime_error("Couldn't find a TKDTree object in the file");
}
FourDHist_Adaptive::FourDHist_Adaptive(const FourDHist_Adaptive& orig)
{
  binner = orig.binner;
  nbins = orig.nbins;
  bincontent = orig.bincontent;
}
void FourDHist_Adaptive::Initialise()
{
  nbins = binner->GetNNodes()+1;
  cout << "Adaptively-binned 4D histogram with " << nbins << " bins" << endl;
  for(int ibin = nbins; ibin-->0;)
    bincontent.push_back(0);
  Clear();
}
int FourDHist_Adaptive::FindBin(double w, double x, double y, double z)
{
  double point[] = {w,x,y,z};
  return binner->FindNode(point) - binner->GetNNodes();
}
bool FourDHist_Adaptive::IsCompatible(const FourDHist_Adaptive& other)
{
  return binner == other.binner && nbins == other.nbins;
}
TTree* FourDHist_Adaptive::SaveToTree()
{
  TTree* tree = new TTree("AccTree","");
  double content; tree->Branch("content",&content);
  for(auto binc : bincontent)
  {
    content = binc;
    tree->Fill();
  }
  return tree;
}
void FourDHist_Adaptive::LoadFromTree(TTree* tree)
{
  double content; tree->SetBranchAddress("content",&content);
  if(tree->GetEntries() == nbins)
    for(int ibin = 0; ibin < nbins; ibin++)
    {
      tree->GetEntry(ibin);
      bincontent[ibin] = content;
    }
  else
    throw std::out_of_range("Tree does not contain the right number of bins");
}
