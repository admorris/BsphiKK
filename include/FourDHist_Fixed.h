#ifndef __FourDHist_Fixed_h__
#define __FourDHist_Fixed_h__
#include <string>
#include "TAxis.h"
#include "TH1D.h"
#include "TH2D.h"
#include "FourDHist.h"
using std::string;
class FourDHist_Fixed : public FourDHist
{
  public:
    FourDHist_Fixed(int,double,double,
              int,double,double,
              int,double,double,
              int,double,double); // nbins, low, high ×4
    FourDHist_Fixed(int,double*,
              int,double*,
              int,double*,
              int,double*); // nbins, array of bin edges [nbins+1] ×4
    FourDHist_Fixed(const FourDHist_Fixed&);
    void Initialise(int,int,int,int);
    bool IsCompatible(const FourDHist_Fixed&);
    void SetAxisNames(string,string,string,string);
    void SetAxisTitles(string,string,string,string);
    TH1D* Project(int); // dim
    TH2D* Project(int,int); // dim1,dim2
  protected:
    TAxis waxis;
    TAxis xaxis;
    TAxis yaxis;
    TAxis zaxis;
  private:
    int FindBin(double,double,double,double); // w,x,y,z
    int GetBin(int,int,int,int); // binw,binx,biny,binz
};
FourDHist_Fixed operator+ (FourDHist_Fixed lhs, const FourDHist_Fixed& rhs) { lhs += rhs; return lhs; }
FourDHist_Fixed operator- (FourDHist_Fixed lhs, const FourDHist_Fixed& rhs) { lhs -= rhs; return lhs; }
FourDHist_Fixed operator* (FourDHist_Fixed lhs, const FourDHist_Fixed& rhs) { lhs *= rhs; return lhs; }
FourDHist_Fixed operator/ (FourDHist_Fixed lhs, const FourDHist_Fixed& rhs) { lhs /= rhs; return lhs; }
#endif

