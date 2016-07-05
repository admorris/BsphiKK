#include "FourDHist.h"
#include <stdexcept>
#include <assert.h>
/*
       ,~~.
      (  6 )-_,
 (\___ )=='-'
  \ .   ) )
   \ `-' /
~'`~'`~'`~'`~
*/
using namespace std;
// Constructor
FourDHist::FourDHist()
{

}
FourDHist::FourDHist(int nbinsw, double wlo, double whi,
                     int nbinsx, double xlo, double xhi,
                     int nbinsy, double ylo, double yhi,
                     int nbinsz, double zlo, double zhi)
{
  waxis = TAxis(nbinsw, wlo, whi);
  xaxis = TAxis(nbinsx, xlo, xhi);
  yaxis = TAxis(nbinsy, ylo, yhi);
  zaxis = TAxis(nbinsz, zlo, zhi);
  Initialise(nbinsw, nbinsx, nbinsy, nbinsz);
}
FourDHist::FourDHist(int nbinsw, double* wbins,
                     int nbinsx, double* xbins,
                     int nbinsy, double* ybins,
                     int nbinsz, double* zbins)
{
  waxis = TAxis(nbinsw, wbins);
  xaxis = TAxis(nbinsx, xbins);
  yaxis = TAxis(nbinsy, ybins);
  zaxis = TAxis(nbinsz, zbins);
  Initialise(nbinsw, nbinsx, nbinsy, nbinsz);
}
void FourDHist::Initialise(int nbinsw, int nbinsx, int nbinsy, int nbinsz)
{
  if(!nbinsw || !nbinsx || !nbinsy || !nbinsz)
    throw std::out_of_range ("Can't have 0 bins in any dimension");
  nbins = nbinsw*nbinsx*nbinsy*nbinsz;
  bincontent = new double[nbins];
  Clear();
  waxis.SetNameTitle("w","w");
  xaxis.SetNameTitle("x","x");
  yaxis.SetNameTitle("y","y");
  zaxis.SetNameTitle("z","z");
}
// Destructor
FourDHist::~FourDHist()
{
  delete[] bincontent;
}
// Copy constructor
FourDHist::FourDHist(const FourDHist& orig)
{
  waxis = orig.waxis;
  xaxis = orig.xaxis;
  yaxis = orig.yaxis;
  zaxis = orig.zaxis;
  nbins = orig.nbins;
  bincontent = new double[nbins];
  for(int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = orig.bincontent[ibin];
}
// Names
void FourDHist::SetAxisNames(string wname
                            ,string xname
                            ,string yname
                            ,string zname)
{
  waxis.SetName(wname.c_str());
  xaxis.SetName(xname.c_str());
  yaxis.SetName(yname.c_str());
  zaxis.SetName(zname.c_str());
}
// Titles
void FourDHist::SetAxisTitles(string wtitle
                             ,string xtitle
                             ,string ytitle
                             ,string ztitle)
{
  waxis.SetTitle(wtitle.c_str());
  xaxis.SetTitle(xtitle.c_str());
  yaxis.SetTitle(ytitle.c_str());
  zaxis.SetTitle(ztitle.c_str());
}
// Find a bin
int FourDHist::FindBin(double w, double x, double y, double z)
{
  int binw, binx, biny, binz;
  binw = waxis.FindBin(w)-1;
  binx = xaxis.FindBin(x)-1;
  biny = yaxis.FindBin(y)-1;
  binz = zaxis.FindBin(z)-1;
  if(binw == -1 || binw >= waxis.GetNbins()
  || binx == -1 || binx >= xaxis.GetNbins()
  || biny == -1 || biny >= yaxis.GetNbins()
  || binz == -1 || binz >= zaxis.GetNbins())
  {
    return -1;
  }
  return GetBin(binw,binx,biny,binz);
}
// Get overall bin index from individual axis indices
int FourDHist::GetBin(int binw, int binx, int biny, int binz)
{
  return binw + waxis.GetNbins() * (binx + xaxis.GetNbins() * (biny + yaxis.GetNbins() * (binz)));
}
void FourDHist::Fill(double w, double x, double y, double z)
{
  int bin = FindBin(w,x,y,z);
  if(bin >=0 && bin < nbins)
    bincontent[bin]++;
}
void FourDHist::Clear()
{
  for(int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = 0;
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
// Comparison of bins and ranges to allow arithmetic
bool FourDHist::IsCompatible(const FourDHist& other)
{
  if(nbins != other.nbins)
    return false; // Remember kids, only you can prevent segmentation faults
  return waxis.GetNbins() == other.waxis.GetNbins() && waxis.GetXmin() == other.waxis.GetXmin() && waxis.GetXmax() == other.waxis.GetXmax()
      && xaxis.GetNbins() == other.xaxis.GetNbins() && xaxis.GetXmin() == other.xaxis.GetXmin() && xaxis.GetXmax() == other.xaxis.GetXmax()
      && yaxis.GetNbins() == other.yaxis.GetNbins() && yaxis.GetXmin() == other.yaxis.GetXmin() && yaxis.GetXmax() == other.yaxis.GetXmax()
      && zaxis.GetNbins() == other.zaxis.GetNbins() && zaxis.GetXmin() == other.zaxis.GetXmin() && zaxis.GetXmax() == other.zaxis.GetXmax();
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
        break;
      case 1:
        bincontent[ibin] -= other.bincontent[ibin];
        break;
      case 2:
        bincontent[ibin] *= other.bincontent[ibin];
        break;
      case 3:
        bincontent[ibin] /= other.bincontent[ibin];
        break;
      default:
        return false;
    }
  }
  return true;
}
// 1D projection
TH1D* FourDHist::Project(int axisindex)
{
  assert(axisindex >= 0 && axisindex < 4);
  TH1D* hist; // the return quantity
  // Axis pointers
  TAxis* axes[] =
  {
     &waxis
    ,&xaxis
    ,&yaxis
    ,&zaxis
  };
  TAxis* projaxis
      ,* intaxis1
      ,* intaxis2
      ,* intaxis3;
  // Choose which axis is to be projected on, and which are to be integrated over.
  projaxis = axes[axisindex];
  intaxis1 = axes[(axisindex+1)%4];
  intaxis2 = axes[(axisindex+2)%4];
  intaxis3 = axes[(axisindex+3)%4];
  // Now we know the projection axis, we can make the hist object
  hist = new TH1D(("projection1D_"+(string)projaxis->GetName()).c_str(),"",projaxis->GetNbins(),projaxis->GetXmin(),projaxis->GetXmax());
  hist->GetXaxis()->SetTitle(projaxis->GetTitle());
  // Bin interators
  int binw,binx,biny,binz;
  int* bins[] =
  {
     &binw
    ,&binx
    ,&biny
    ,&binz
  };
  // Choose which iterator is assigned to which axis
  int* ibin = bins[axisindex];
  int* i1 = bins[(axisindex+1)%4];
  int* i2 = bins[(axisindex+2)%4];
  int* i3 = bins[(axisindex+3)%4];
  for(*ibin = 0; *ibin < projaxis->GetNbins(); *ibin += 1) // Use += instead of ++ because of operator precedence!
  {
    double bintegral = 0; // bin integral :)
    // Integrate over the axes that aren't being projected
    for(*i1 = 0; *i1 < intaxis1->GetNbins(); *i1 += 1)
    {
      for(*i2 = 0; *i2 < intaxis2->GetNbins(); *i2 += 1)
      {
        for(*i3 = 0; *i3 < intaxis3->GetNbins(); *i3 += 1)
        {
          bintegral += bincontent[GetBin(binw,binx,biny,binz)];
        }
      }
    }
    // Fill the 1D histogram
    hist->SetBinContent(*ibin+1,bintegral);
  }
  return hist;
}
// 2D projection
TH2D* FourDHist::Project(int axisindex1, int axisindex2)
{
  assert(axisindex1 != axisindex2);
  assert(axisindex1 >= 0 && axisindex1 < 4);
  assert(axisindex2 >= 0 && axisindex2 < 4);
  TH2D* hist; // the return quantity
  // Axis pointers
  TAxis* axes[] =
  {
     &waxis
    ,&xaxis
    ,&yaxis
    ,&zaxis
  };
  TAxis* projaxis1
      ,* projaxis2
      ,* intaxis1
      ,* intaxis2;
  // Choose which axes are to be projected on, and which are to be integrated over.
  projaxis1 = axes[axisindex1];
  projaxis2 = axes[axisindex2];
  int iaxis, iint1(0);
  for(iaxis = 0; iaxis == axisindex1 || iaxis == axisindex2; iaxis++)
    iint1 = iaxis+1;
  int iint2(iint1+1);
  for(iaxis = iint1+1; iaxis == axisindex1 || iaxis == axisindex2; iaxis++)
    iint2 = iaxis+1;
  assert(iint1 != axisindex1 && iint1 != axisindex2);
  assert(iint2 != axisindex1 && iint2 != axisindex2);
  assert(iint1 != iint2);
  assert(iint1 < 4);
  assert(iint2 < 4);
  intaxis1 = axes[iint1];
  intaxis2 = axes[iint2];
  // Now we know the projection axis, we can make the hist object
  hist = new TH2D
  (
     ("projection2D_"+(string)projaxis1->GetName()+"-"+(string)projaxis2->GetName()).c_str()
    ,""
    ,projaxis1->GetNbins(),projaxis1->GetXmin(),projaxis1->GetXmax()
    ,projaxis2->GetNbins(),projaxis2->GetXmin(),projaxis2->GetXmax()
  );
  hist->GetXaxis()->SetTitle(projaxis1->GetTitle());
  hist->GetYaxis()->SetTitle(projaxis2->GetTitle());
  // Bin interators
  int binw,binx,biny,binz;
  int* bins[] =
  {
     &binw
    ,&binx
    ,&biny
    ,&binz
  };
  // Choose which iterator is assigned to which axis
  int* ibin1 = bins[axisindex1];
  int* ibin2 = bins[axisindex2];
  int* i1 = bins[iint1];
  int* i2 = bins[iint2];
  for(*ibin1 = 0; *ibin1 < projaxis1->GetNbins(); *ibin1 += 1) // Use += instead of ++ because of operator precedence!
  {
    for(*ibin2 = 0; *ibin2 < projaxis2->GetNbins(); *ibin2 += 1)
    {
      double bintegral = 0; // bin integral :)
      // Integrate over the axes that aren't being projected
      for(*i1 = 0; *i1 < intaxis1->GetNbins(); *i1 += 1)
      {
        for(*i2 = 0; *i2 < intaxis2->GetNbins(); *i2 += 1)
        {
          bintegral += bincontent[GetBin(binw,binx,biny,binz)];
        }
      }
      // Fill the 2D histogram
      hist->SetBinContent(*ibin1+1,*ibin2+1,bintegral);
    }
  }
  return hist;
}

