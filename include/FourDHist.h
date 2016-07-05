#ifndef __FourDHist_h__
#define __FourDHist_h__
#include <string>
#include "TAxis.h"
#include "TH1D.h"
#include "TH2D.h"
using std::string;
class FourDHist
{
  public:
    FourDHist();
    FourDHist(int,double,double,
              int,double,double,
              int,double,double,
              int,double,double); // nbins, low, high ×4
    FourDHist(int,double*,
              int,double*,
              int,double*,
              int,double*); // nbins, array of bin edges [nbins+1] ×4
    FourDHist(const FourDHist&);
    ~FourDHist();
    void Initialise(int,int,int,int);
    void SetAxisNames(string,string,string,string);
    void SetAxisTitles(string,string,string,string);
    void Fill(double,double,double,double); // w,x,y,z
    void Clear();
    double MaxBinContent();
    double MinBinContent();
    TH1D* BinContentHist();
    bool Add(const FourDHist&);
    bool Subtract(const FourDHist&);
    bool Multiply(const FourDHist&);
    bool Divide(const FourDHist&);
    bool IsCompatible(const FourDHist&);
    FourDHist operator= (const FourDHist& rhs) { return FourDHist(rhs); }
    bool      operator+=(const FourDHist& rhs) { return Add(rhs); }
    bool      operator-=(const FourDHist& rhs) { return Subtract(rhs); }
    bool      operator*=(const FourDHist& rhs) { return Multiply(rhs); }
    bool      operator/=(const FourDHist& rhs) { return Divide(rhs); }
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
    bool Arithmetic(const FourDHist&,int);// 0-3
    int FindBin(double,double,double,double); // w,x,y,z
    int GetBin(int,int,int,int); // binw,binx,biny,binz
};
FourDHist operator+ (FourDHist lhs, const FourDHist& rhs) { lhs += rhs; return lhs; }
FourDHist operator- (FourDHist lhs, const FourDHist& rhs) { lhs -= rhs; return lhs; }
FourDHist operator* (FourDHist lhs, const FourDHist& rhs) { lhs *= rhs; return lhs; }
FourDHist operator/ (FourDHist lhs, const FourDHist& rhs) { lhs /= rhs; return lhs; }
#endif

