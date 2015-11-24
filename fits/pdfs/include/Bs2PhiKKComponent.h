#ifndef __BS2PHIKKCOMPONENT_H__
#define __BS2PHIKKCOMPONENT_H__
#include "TComplex.h"
#include "DPMassShape.hh"
#include <string>
#include <vector>
#include "DPBarrierFactor.hh"
using std::string;
using std::vector;
class Bs2PhiKKComponent
{
  public:
    Bs2PhiKKComponent(int, double, double, string); // J2, M2, W2
    void SetParameters(vector<TComplex>); 
    TComplex Amplitude(double, double, double, double); // KK_M, Phi_angle, cos_theta1, cos_theta2
  private:
    TComplex*        A(int);                    // Polarisation amplitude coefficients
    TComplex         F(double, double, double); // Angular part
    TComplex         M(double);                 // Mass-dependent part (KK resonance shape)
    vector<TComplex> _A;
    int              _J1; // Spin of particle 1 (P-wave, 1)
    int              _J2; // Spin of particle 2 (0, 1 or 2)
    double           _M1; // Mass of particle 1 (phi mass)
    double           _M2; // Mass of particle 2
    double           _W1; // Width of particle 1 (phi width)
    double           _W2; // Width of particle 2
    int              _lambda_max; // Keep track of the max. helicity value
    DPMassShape*     _M; // Pointer to resonance shape function
    DPBarrierFactor* barrier; // Blatt-Weisskopf barrier penetration factor for particle 2
};
#endif
