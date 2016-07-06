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
