#include "SphericalHarmonic.h"
#include "TMath.h"
#include "TComplex.h"
#include "Math/SpecFuncMathMore.h"
#include <iostream>
using std::cout;
using std::endl;

TComplex Y(int l, int m, double cos_theta, double phi)
{
  if(l < 0)
  {
    cout << "Negative value of l = " << l << endl;
    return TComplex(0,0);
  }
  if(m > l || m < -l)
  {
    cout << "Unphysical value: m = " << m << " for l = " << l << endl;
    return TComplex(0,0);
  }
  if(cos_theta < -1 || cos_theta > 1)
  {
    cout << "Impossible value: cos(theta) = " << cos_theta << endl;
    return TComplex(0,0);
  }
  double c = 1; // Proportionality constant to turn P^{m}_{l} into P^{-m}_{l}
  if(m<0)
  {
    c = TMath::Power(-1,m) * TMath::Factorial(l-m) / TMath::Factorial(l+m);
  }
  return c*ROOT::Math::sph_legendre(l,TMath::Abs(m),cos_theta)*TComplex::Exp(TComplex::I()*m*phi);
}
