#include "SphericalHarmonic.h"
#include "TMath.h"
#include "TComplex.h"
#include "Math/SpecFuncMathMore.h"
#include <iostream>
using std::cout;
using std::endl;

TComplex SphericalHarmonic::Y(int l, int m, double ctheta, double phi)
{
  if(l < 0)
  {
    cout << "SphericalHarmonic::Y Negative value of l = " << l << endl;
    return TComplex(0,0);
  }
  if(m > l || m < -l)
  {
    cout << "SphericalHarmonic::Y Unphysical value: m = " << m << " for l = " << l << endl;
    return TComplex(0,0);
  }
  if(ctheta < -1 || ctheta > 1)
  {
    cout << "SphericalHarmonic::Y Impossible value: cos(theta) = " << ctheta << endl;
    return TComplex(0,0);
  }
  double c = 1; // Proportionality constant to turn P^{m}_{l} into P^{-m}_{l}
  if(m<0)
  {
    c = TMath::Power(-1,m) * TMath::Factorial(l-m) / TMath::Factorial(l+m);
  }
  return c*ROOT::Math::sph_legendre(l,TMath::Abs(m),ctheta)*TComplex::Exp(TComplex::I()*m*phi);
}
