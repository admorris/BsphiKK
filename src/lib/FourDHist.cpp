#include "FourDHist.h"
#include <stdexcept>
#include <assert.h>
#include "itoa.h"
using namespace std;
void FourDHist::Initialise()
{
  for(int ibin = nbins; ibin-->0;)
    bincontent.push_back(0);
}
// Destructor
FourDHist::~FourDHist()
{
}
int FourDHist::FindBin(double w, double x, double y, double z)
{
  return 0;
}
void FourDHist::Fill(double w, double x, double y, double z)
{
  int bin = this->FindBin(w,x,y,z); // make sure you get the function from the right class
  if(bin <0)
    under++;
  else if(bin >= nbins)
    over++;
  else
    bincontent[bin]++;
}
double FourDHist::Eval(double w, double x, double y, double z)
{
  return bincontent[FindBin(w,x,y,z)];
}
void FourDHist::Clear()
{
  for(auto &binc: bincontent)
    binc = 0;
  under = 0;
  over = 0;
}
double FourDHist::MaxBinContent()
{
  double maxbincontent = bincontent[0];
  for(auto binc: bincontent)
    if(binc > maxbincontent)
      maxbincontent = binc;
  return maxbincontent;
}
double FourDHist::MinBinContent()
{
  double minbincontent = bincontent[0];
  for(auto binc: bincontent)
    if(binc < minbincontent)
      minbincontent = binc;
  return minbincontent;
}
TH1D* FourDHist::BinContentHist()
{
  TH1D* hist = new TH1D("BinContentHist","",100,MinBinContent(),MaxBinContent());
  for(auto binc: bincontent)
    hist->Fill(binc);
  return hist;
}

// Arithmetic with histograms
bool FourDHist::Add(const FourDHist& other)
{
  return Arithmetic(other,0);
}
bool FourDHist::Subtract(const FourDHist& other)
{
  return Arithmetic(other,1);
}
bool FourDHist::Multiply(const FourDHist& other)
{
  return Arithmetic(other,2);
}
bool FourDHist::Divide(const FourDHist& other)
{
  return Arithmetic(other,3);
}
bool FourDHist::IsCompatible(const FourDHist& other)
{
  return nbins == other.nbins;
}
bool FourDHist::Arithmetic(const FourDHist& other,int op)
{
  if(!IsCompatible(other))
  {
    throw runtime_error("FourDHist ERROR: Histograms must have the same ranges and binning schemes to do arithmetic.");
    return false;
  }
  switch(op)
  {
    case 0:
      for(int ibin = 0; ibin < nbins; ibin++)
        bincontent[ibin] += other.bincontent[ibin];
      under += other.under;
      over += other.over;
      break;
    case 1:
      for(int ibin = 0; ibin < nbins; ibin++)
        bincontent[ibin] -= other.bincontent[ibin];
      under -= other.under;
      over -= other.over;
      break;
    case 2:
      for(int ibin = 0; ibin < nbins; ibin++)
        bincontent[ibin] *= other.bincontent[ibin];
      under *= other.under;
      over *= other.over;
      break;
    case 3:
      for(int ibin = 0; ibin < nbins; ibin++)
        bincontent[ibin] /= other.bincontent[ibin];
      under /= other.under;
      over /= other.over;
      break;
    default:
      return false;
  }
  return true;
}
void FourDHist::Print()
{
  for(auto binc: bincontent)
    printf("%f\n",binc);
}
double FourDHist::Integral()
{
  double sum = 0;
  for(auto binc: bincontent)
    sum+=binc;
  return sum;
}
