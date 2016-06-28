#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <assert.h>

#include "TAxis.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

#include "GetTree.h"

using namespace std;

class FourDHist
{
  public:
//    FourDHist();
    FourDHist(int,double,double,
              int,double,double,
              int,double,double,
              int,double,double); // nbins, low, high ×4
    FourDHist(const FourDHist&);
    ~FourDHist();
    void SetAxisNames(string,string,string,string);
    void SetAxisTitles(string,string,string,string);
    void Fill(double,double,double,double); // w,x,y,z
    TH1D* Project(int); // dim
    TH2D* Project(int,int); // dim1,dim2
  protected:
    int nbins;
    double* bincontent;
    TAxis waxis;
    TAxis xaxis;
    TAxis yaxis;
    TAxis zaxis;
  private:
    int FindBin(double,double,double,double); // w,x,y,z
    int GetBin(int,int,int,int); // binw,binx,biny,binz
};
//FourDHist::FourDHist();
FourDHist::FourDHist(int nbinsw, double wlo, double whi,
                     int nbinsx, double xlo, double xhi,
                     int nbinsy, double ylo, double yhi,
                     int nbinsz, double zlo, double zhi)
{
  if(!nbinsw || !nbinsx || !nbinsy || !nbinsz)
    throw std::out_of_range ("Can't have 0 bins in any dimension");
  nbins = nbinsw*nbinsx*nbinsy*nbinsz;
  for(int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = 0;
  bincontent = new double[nbins];
  waxis = TAxis(nbinsw, wlo, whi);
  xaxis = TAxis(nbinsx, xlo, xhi);
  yaxis = TAxis(nbinsy, ylo, yhi);
  zaxis = TAxis(nbinsz, zlo, zhi);
  waxis.SetNameTitle("w","w");
  xaxis.SetNameTitle("x","x");
  yaxis.SetNameTitle("y","y");
  zaxis.SetNameTitle("z","z");
  cout << "FourDHist object constructed" << endl;
}
FourDHist::~FourDHist()
{
  cout << "Deleting FourDHist object" << endl;
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
  for(int ibin = 0; ibin < nbins; ibin++)
    bincontent[ibin] = orig.bincontent[ibin];
}
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
  cout << "Projecting onto axis " << projaxis->GetName() << endl;
  // Now we know the projection axis, we can make the hist object
  hist = new TH1D(("projection_"+(string)projaxis->GetName()).c_str(),"",projaxis->GetNbins(),projaxis->GetXmin(),projaxis->GetXmax());
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
  cout << "Integrating..." << endl;
  for(*ibin = 0; *ibin < projaxis->GetNbins(); *ibin += 1) // Use += instead of ++ because of operator precedence!
  {
    cout << *ibin << "/" << projaxis->GetNbins() << endl; 
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
    hist->SetBinContent(*ibin,bintegral);
  }
  cout << "Done" << endl;
  return hist;
}
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
  cout << "Projecting onto axes " << projaxis1->GetName() << " and " << projaxis2->GetName() << endl;
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
     ("projection_"+(string)projaxis1->GetName()+"-"+(string)projaxis2->GetName()).c_str()
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
  cout << "Integrating..." << endl;
  cout << "x\ty" << endl;
  for(*ibin1 = 0; *ibin1 < projaxis1->GetNbins(); *ibin1 += 1) // Use += instead of ++ because of operator precedence!
  {
    for(*ibin2 = 0; *ibin2 < projaxis2->GetNbins(); *ibin2 += 1)
    {
      cout << *ibin1 << "/" << projaxis1->GetNbins() << "\t" << *ibin2 << "/" << projaxis2->GetNbins() << endl; 
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
      hist->SetBinContent(*ibin1,*ibin2,bintegral);
    }
  }
  cout << "Done" << endl;
  return hist;
}
// End of class definition. Now to do something with it.
void AngularAcceptance(string filename)
{
  bool sym = true;
  TTree* tree = GetTree(filename);
  int n = tree->GetEntries();
  cout << "Tree contains " << n << " entries" << endl;
  double x[4];
  tree->SetBranchAddress("Phi_angle" ,&x[0]);
  tree->SetBranchAddress("cos_theta1",&x[1]);
  tree->SetBranchAddress("cos_theta2",&x[2]);
  tree->SetBranchAddress("KK_M"      ,&x[3]);
  cout << "Branches set" << endl;
  TFile output("AcceptanceProjections.root","RECREATE");
  FourDHist hist
  (
     5,sym? 0 : -TMath::Pi(), TMath::Pi()
    ,5,sym? 0 : -1, 1
    ,5,sym? 0 : -1, 1
    ,5,493*2,1800
  );
  hist.SetAxisNames("phi","ctheta_1","ctheta_2","mKK");
  hist.SetAxisTitles("#Phi","#cos(#theta_{1})","#cos(#theta_{2})","m(K^{#plus}K^{#minus})");
  cout << "Entering event loop" << endl;
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    sym ?
      hist.Fill(TMath::Abs(x[0]),TMath::Abs(x[1]),TMath::Abs(x[2]),TMath::Abs(x[3]))
      :
      hist.Fill(x[0],x[1],x[2],x[3]);
  }
  for(int i = 0; i < 4; i++)
  {
    hist.Project(i);
    for(int j = i+1; j < 4; j++)
      hist.Project(i,j);
  }
  cout << "End of AngularAcceptance()" << endl;
  delete tree;
  cout << "Deleted tree" << endl;
}
int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1]);
  cout << "End of main()" << endl;
  return 0;
}

