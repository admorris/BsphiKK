#ifndef __SPHERICALHARMONIC_H__
#define __SPHERICALHARMONIC_H__
#include "TComplex.h"
class SphericalHarmonic
{
  static TComplex Y(int, int, double, double); // l, m, cos_theta, phi
};
// l is spin
// m is spin projection -l < m < +l
// cos_theta is cosine of polar angle
// phi is azimuthal angle
#endif
