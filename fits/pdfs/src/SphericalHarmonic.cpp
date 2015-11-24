#include "SphericalHarmonic.h"
#include "TMath.h"
#include "TComplex.h"
#include "Math/SpecFuncMathMore.h"

TComplex Y(int l, int m, double theta, double phi)
{
  return ROOT::Math::sph_legendre(l,m,theta)*TComplex::Exp(TComplex::I()*m*phi);
}
