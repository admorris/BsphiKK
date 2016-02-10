#include "TMath.h"
#include "Bs2PhiKKNonResonant.h"
double Bs2PhiKKNonResonant::Evaluate(double mKK)
{
  const double mK   = Bs2PhiKKComponent::mK;
  const double mBs  = Bs2PhiKKComponent::mBs;
  const double mPhi = Bs2PhiKKComponent::mphi;
  double t1 = mKK*mKK-(2*mK)*(2*mK);
  double t2 = mKK*mKK;
  double t31 = mBs*mBs - (mKK + mPhi)*(mKK + mPhi);
  double t32 = mBs*mBs - (mKK - mPhi)*(mKK - mPhi);
  double p1_st = sqrt(t1*t2)/mKK/2.;
  double p3    = sqrt(t31*t32)/mBs/2.;
  double eval = p1_st*p3/1e6;
  return TMath::IsNaN(eval) ? 0 : eval;
}
