#include "FourDHist.h"
#include <stdexcept>
#include <assert.h>
#include "itoa.h"
using namespace std;
void FourDHist::Initialise()
{
  bincontent = new double[nbins];
}
// Destructor
FourDHist::~FourDHist()
{
  delete[] bincontent;
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
  for(int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = 0;
  under = 0;
  over = 0;
}
double FourDHist::MaxBinContent()
{
  double maxbincontent = bincontent[0];
  for(int ibin = 1; ibin < nbins; ibin++)
    if(bincontent[ibin] > maxbincontent)
      maxbincontent = bincontent[ibin];
  return maxbincontent;
}
double FourDHist::MinBinContent()
{
  double minbincontent = bincontent[0];
  for(int ibin = 1; ibin < nbins; ibin++)
    if(bincontent[ibin] < minbincontent)
      minbincontent = bincontent[ibin];
  return minbincontent;
}
TH1D* FourDHist::BinContentHist()
{
  TH1D* hist = new TH1D("BinContentHist","",100,MinBinContent(),MaxBinContent());
  for(int ibin = 0; ibin < nbins; ibin++)
    hist->Fill(bincontent[ibin]);
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
  for(int ibin = 0; ibin < nbins; ibin++)
  {
    switch(op)
    {
      case 0:
        bincontent[ibin] += other.bincontent[ibin];
        under += other.under;
        over += other.over;
        break;
      case 1:
        bincontent[ibin] -= other.bincontent[ibin];
        under -= other.under;
        over -= other.over;
        break;
      case 2:
        bincontent[ibin] *= other.bincontent[ibin];
        under *= other.under;
        over *= other.over;
        break;
      case 3:
        bincontent[ibin] /= other.bincontent[ibin];
        under /= other.under;
        over /= other.over;
        break;
      default:
        return false;
    }
  }
  return true;
}
void FourDHist::Print()
{
  for(int ibin = 0; ibin < nbins; ibin++)
    printf("%f\n",bincontent[ibin]);
}
double FourDHist::Integral()
{
  double sum = 0;
  for(int ibin = 0; ibin < nbins; ibin++)
    sum+=bincontent[ibin];
  return sum;
}
