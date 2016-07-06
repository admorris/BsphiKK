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
    ~FourDHist();
    virtual void Initialise();
    void Fill(double,double,double,double); // w,x,y,z
    void Clear();
    double Eval(double,double,double,double);
    double MaxBinContent();
    double MinBinContent();
    double UnderFlow() { return under; }
    double OverFlow() { return over; }
    double Integral();
    TH1D* BinContentHist();
    bool Add(const FourDHist&);
    bool Subtract(const FourDHist&);
    bool Multiply(const FourDHist&);
    bool Divide(const FourDHist&);
    virtual bool IsCompatible(const FourDHist&);
    void Print();
    FourDHist operator= (const FourDHist& rhs) { return FourDHist(rhs); }
    bool      operator+=(const FourDHist& rhs) { return Add(rhs); }
    bool      operator-=(const FourDHist& rhs) { return Subtract(rhs); }
    bool      operator*=(const FourDHist& rhs) { return Multiply(rhs); }
    bool      operator/=(const FourDHist& rhs) { return Divide(rhs); }
  protected:
    int nbins;
    double under;
    double over;
    double* bincontent;
  private:
    bool Arithmetic(const FourDHist&,int);// 0-3
    virtual int FindBin(double,double,double,double); // w,x,y,z
};
FourDHist operator+ (FourDHist lhs, const FourDHist& rhs) { lhs += rhs; return lhs; }
FourDHist operator- (FourDHist lhs, const FourDHist& rhs) { lhs -= rhs; return lhs; }
FourDHist operator* (FourDHist lhs, const FourDHist& rhs) { lhs *= rhs; return lhs; }
FourDHist operator/ (FourDHist lhs, const FourDHist& rhs) { lhs /= rhs; return lhs; }
#endif

