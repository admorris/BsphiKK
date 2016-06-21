#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "TAxis.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"

using namespace std;

class FourDHist
{
  public:
//    FourDHist();
    FourDHist(unsigned int,double,double,
              unsigned int,double,double,
              unsigned int,double,double,
              unsigned int,double,double); // nbins, low, high ×4
    FourDHist(const FourDHist&);
    ~FourDHist();
    void SetAxisNames(string,string,string,string);
    void Fill(double,double,double,double); // w,x,y,z
    TH1D* Project(unsigned short); // dim
    TH2D* Project(unsigned short,unsigned short); // dim1,dim2
  protected:
    unsigned int nbins;
    double* bincontent;
    TAxis waxis;
    TAxis xaxis;
    TAxis yaxis;
    TAxis zaxis;
    string wname;
    string xname;
    string yname;
    string zname;
  private:
    int FindBin(double,double,double,double); // w,x,y,z
    int GetBin(unsigned int,unsigned int,unsigned int,unsigned int); // binw,binx,biny,binz
};
//FourDHist::FourDHist();
FourDHist::FourDHist(unsigned int nbinsw, double wlo, double whi,
                     unsigned int nbinsx, double xlo, double xhi,
                     unsigned int nbinsy, double ylo, double yhi,
                     unsigned int nbinsz, double zlo, double zhi)
{
  if(!nbinsw || !nbinsx || !nbinsy || !nbinsz)
    throw std::out_of_range ("Can't have 0 bins in any dimension");
  nbins = nbinsw*nbinsx*nbinsy*nbinsz;
  for(unsigned int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = 0;
  bincontent = new double[nbins];
  waxis = TAxis(nbinsw, wlo, whi);
  xaxis = TAxis(nbinsx, xlo, xhi);
  yaxis = TAxis(nbinsy, ylo, yhi);
  zaxis = TAxis(nbinsz, zlo, zhi);
  wname = "w";
  xname = "x";
  yname = "y";
  zname = "z";
}
FourDHist::~FourDHist()
{
  delete[] bincontent;
}
FourDHist::FourDHist(const FourDHist& orig)
{
  waxis = orig.waxis;
  xaxis = orig.xaxis;
  yaxis = orig.yaxis;
  zaxis = orig.zaxis;
  nbins = orig.nbins;
  bincontent = new double[nbins];
  for(unsigned int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = orig.bincontent[ibin];
}
void SetAxisNames(string newwname
                 ,string newxname
                 ,string newyname
                 ,string newzname)
{
  wname = newwname;
  xname = newxname;
  yname = newyname;
  zname = newzname;
}
int FourDHist::FindBin(double w, double x, double y, double z)
{
  unsigned int bin;
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
int FourDHist::GetBin(unsigned int binw, unsigned int binx, unsigned int biny, unsigned int binz)
{
  bin = binw + waxis.GetNbins() * (binx + xaxis.GetNbins() * (biny + yaxis.GetNbins() * (binz)));
  return bin;
}
void FourDHist::Fill(double w, double x, double y, double z)
{
  int bin = FindBin(w,x,y,z);
  if(bin >=0 && bin < nbins)
    bincontent[bin]++;
}
TH1D* FourDHist::Project(unsigned int axisindex)
{
  if(axisindex>=4)
    return new TH1D();
  TH1D* hist; // return quantity
  // Dish out the axes
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
  projaxis = axes[axisindex];
  intaxis1 = axes[(axisindex+1)%4];
  intaxis2 = axes[(axisindex+2)%4];
  intaxis3 = axes[(axisindex+3)%4];
  // Now we know the projection axis, we can make the hist object
  hist = new TH1D(("projection").c_str(),"",projaxis->GetNbins(),projaxis->GetXmin(),projaxis->GetXmax());
  // Assign the bin indices
  unsigned int binw,binx,biny,binz;
  unsigned int* bins[] =
  {
     &binw
    ,&binx
    ,&biny
    ,&binz
  }
  unsigned int* ibin = bins[axisindex];
  unsigned int* i1 = bins[(axisindex+1)%4];
  unsigned int* i2 = bins[(axisindex+2)%4];
  unsigned int* i3 = bins[(axisindex+3)%4];
  for(*ibin = 0; *ibin < projaxis->GetNbins(); *ibin++)
  {
    double bintegral = 0; // bin integral :)
    for(*i1 = 0; *i1 < intaxis1->GetNbins(); *i1++)
    {
      for(*i2 = 0; *i2 < intaxis2->GetNbins(); *i2++)
      {
        for(*i3 = 0; *i3 < intaxis3->GetNbins(); *i3++)
        {
          bintegral += bincontent[GetBin(binw,binx,biny,binz)];
        }
      }
    }
    hist->SetBinContent(*ibin,bintegral);
  }
  return hist;
}
void AngularAcceptance(string filename)
{
  bool imposesymmetry = true;
  // Input
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  int n = tree->GetEntries();
  double x[3],d;
  tree->SetBranchAddress("Phi_angle", &x[0]);
  tree->SetBranchAddress("cos_theta1",&x[1]);
  tree->SetBranchAddress("cos_theta2",&x[2]);
  // Configure and make the 3D histogram
  int nbinsx = 10;
  double xlo = imposesymmetry ? 0 : -TMath::Pi();
  double xup = +TMath::Pi();
  double xrange = xup - xlo;
  int nbinsy = 10;
  double ylo = imposesymmetry ? 0 : -1;
  double yup = +1;
  double yrange = yup - ylo;
  int nbinsz = 20;
  double zlo = imposesymmetry ? 0 : -1;
  double zup = +1;
  double zrange = zup - zlo;
  // For TH3::Interpolate() to work properly, the points have to be within the centres of the edge bins
  // Fill the histogram
  cout << "Filling a 3D histogram with " << n << " events." << endl;
  progbar bar(n);
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    imposesymmetry ?
      hist->Fill(TMath::Abs(x[0]),TMath::Abs(x[1]),TMath::Abs(x[2]))
      :
      hist->Fill(x[0],x[1],x[2]);
    if(i%(n/100)==0)
    {
      bar.print(i);
    }
  }
}

int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1]);
  return 0;
}

