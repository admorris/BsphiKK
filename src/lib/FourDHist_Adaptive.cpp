#include "FourDHist_Adaptive.h"
#include "TCollection.h"
#include "TKey.h"
#include <stdexcept>
#include <assert.h>
#include <iostream>
using namespace std;
FourDHist_Adaptive::FourDHist_Adaptive(TKDTreeBinning* _b) : binner(_b)
{

}
FourDHist_Adaptive::FourDHist_Adaptive(TFile* _f)
{
  TIter next = _f->GetListOfKeys();
  TKey* key;
  // Look for any TKDTreeBinning object in the file
  while((key = (TKey*)next()))
  {
    if(strcmp(key->GetClassName(),"TKDTreeBinning")==0)
    {
      binner = (TKDTreeBinning*)key->ReadObj();
      Initialise();
    }
  }
}
FourDHist_Adaptive::FourDHist_Adaptive(const FourDHist_Adaptive& orig)
{
  binner = orig.binner;
  nbins = orig.nbins;
  bincontent = new double[nbins];
  for(int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = orig.bincontent[ibin];
}
void FourDHist_Adaptive::Initialise()
{
  nbins = binner->GetNBins();
  cout << "Adaptively-binned 4D histogram with " << nbins << " bins" << endl;
  bincontent = new double[nbins];
  Clear();
}
int FourDHist_Adaptive::FindBin(double w, double x, double y, double z)
{
  double point[] = {w,x,y,z};
  return binner->FindBin(point);
}
bool FourDHist_Adaptive::IsCompatible(const FourDHist_Adaptive& other)
{
  return binner == other.binner && nbins == other.nbins;
}
TTree* FourDHist_Adaptive::SaveToTree()
{
  TTree* tree = new TTree;
  const double* binedgemax,* binedgemin;
  double binhi[4], binlo[4];
  tree->Branch("binwhi" ,&binhi[0]);
  tree->Branch("binwlo" ,&binlo[0]);
  tree->Branch("binxhi" ,&binhi[1]);
  tree->Branch("binxlo" ,&binlo[1]);
  tree->Branch("binyhi" ,&binhi[2]);
  tree->Branch("binylo" ,&binlo[2]);
  tree->Branch("binzhi" ,&binhi[3]);
  tree->Branch("binzlo" ,&binlo[3]);
  double content; tree->Branch("content",&content );
  for(int ibin = 0; ibin < nbins; ibin++)
  {
    binedgemax = binner->GetBinMaxEdges(ibin);
    binedgemin = binner->GetBinMinEdges(ibin);
    for(int idim = 0; idim < 4; idim++)
    {
      binhi[idim] = binedgemax[idim];
      binlo[idim] = binedgemin[idim];
    }
    content = bincontent[ibin];
    tree->Fill();
  }
  return tree;
}
void FourDHist_Adaptive::LoadFromTree(TTree* tree)
{
  if(tree->GetEntries() == nbins)
  {
    double content; tree->SetBranchAddress("content",&content);
    for(int ibin = 0; ibin < nbins; ibin++)
    {
      tree->GetEntry(ibin);
      bincontent[ibin] = content;
    }
  }
  else
    throw std::out_of_range("Tree does not contain the right number of bins");
}
