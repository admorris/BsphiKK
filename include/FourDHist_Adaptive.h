#ifndef __FourDHist_Adaptive_h__
#define __FourDHist_Adaptive_h__
#include <string>
#include "TAxis.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "FourDHist.h"
#include "TKDTreeBinning.h"
using std::string;
class FourDHist_Adaptive : public FourDHist
{
  public:
    FourDHist_Adaptive(TKDTreeBinning*);
    FourDHist_Adaptive(const FourDHist_Adaptive&);
    bool IsCompatible(const FourDHist_Adaptive&);
    TTree* SaveToTree();
  protected:
    TKDTreeBinning* binner;
  private:
    int FindBin(double,double,double,double); // w,x,y,z
};
FourDHist_Adaptive operator+ (FourDHist_Adaptive lhs, const FourDHist_Adaptive& rhs) { lhs += rhs; return lhs; }
FourDHist_Adaptive operator- (FourDHist_Adaptive lhs, const FourDHist_Adaptive& rhs) { lhs -= rhs; return lhs; }
FourDHist_Adaptive operator* (FourDHist_Adaptive lhs, const FourDHist_Adaptive& rhs) { lhs *= rhs; return lhs; }
FourDHist_Adaptive operator/ (FourDHist_Adaptive lhs, const FourDHist_Adaptive& rhs) { lhs /= rhs; return lhs; }
#endif

