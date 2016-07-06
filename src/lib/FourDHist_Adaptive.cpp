#include "FourDHist_Adaptive.h"
#include <stdexcept>
#include <assert.h>
#include <iostream>
using namespace std;
FourDHist_Adaptive::FourDHist_Adaptive(TKDTreeBinning* _b) : binner(_b)
{
  nbins = binner->GetNBins();
  cout << "Adaptively-binned 4D histogram with " << nbins << " bins" << endl;
  bincontent = new double[nbins];
  Clear();
}
FourDHist_Adaptive::FourDHist_Adaptive(const FourDHist_Adaptive& orig)
{
  binner = orig.binner;
  nbins = orig.nbins;
  bincontent = new double[nbins];
  for(int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = orig.bincontent[ibin];
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
  double binwhi;  tree->Branch("binwhi" ,&binhi[0]);
  double binwlo;  tree->Branch("binwlo" ,&binlo[0]);
  double binxhi;  tree->Branch("binxhi" ,&binhi[1]);
  double binxlo;  tree->Branch("binxlo" ,&binlo[1]);
  double binyhi;  tree->Branch("binyhi" ,&binhi[2]);
  double binylo;  tree->Branch("binylo" ,&binlo[2]);
  double binzhi;  tree->Branch("binzhi" ,&binhi[3]);
  double binzlo;  tree->Branch("binzlo" ,&binlo[3]);
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
