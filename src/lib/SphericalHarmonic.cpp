#include "SphericalHarmonic.h"
#include "TComplex.h"
#include "TMath.h"
#include <gsl/gsl_sf_legendre.h>
#include <iostream>
using std::cout;
using std::endl;

TComplex SphericalHarmonic::Y(int l, int m, double ctheta, double phi)
{
  double c = 1; // Proportionality constant to turn P^{m}_{l} into P^{-m}_{l}
  if(m < 0)
  {
    c = pow(-1, m) * TMath::Factorial(l - m) / TMath::Factorial(l + m);
  }
  return c * gsl_sf_legendre_sphPlm(l, abs(m), ctheta) * TComplex(1,m*phi,true);//TComplex::Exp(TComplex::I() * m * phi);
}
