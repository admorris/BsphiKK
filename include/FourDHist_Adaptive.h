#ifndef __FourDHist_Adaptive_h__
#define __FourDHist_Adaptive_h__
#include <string>
#include "TAxis.h"
#include "TH1D.h"
#include "TH2D.h"
#include "FourDHist.h"
#include "TKDTreeBinning.h"
using std::string;
class FourDHist_Adaptive : public FourDHist
{
  public:
    FourDHist_Adaptive(TKDTreeBinning*);
    FourDHist_Adaptive(const FourDHist_Adaptive&);
    bool IsCompatible(const FourDHist_Adaptive&);
    TH1D* Project(int); // dim
    TH2D* Project(int,int); // dim1,dim2
  protected:
    TKDTreeBinning* binner;
    int nbins;
  private:
    int FindBin(double,double,double,double); // w,x,y,z
};
#endif

