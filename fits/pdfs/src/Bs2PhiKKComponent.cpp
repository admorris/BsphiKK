/** @class Bs2PhiKKComponent Bs2PhiKKComponent.cpp
 *
 *  RapidFit PDF for Bs2PhiKKComponent
 *
 *  @author Adam Morris
 *  @date Nov-Dec 2015
 */
// Self 
#include "Bs2PhiKKComponent.h"
// Std Libraries
#include <iostream>
// ROOT Libraries
#include "TMath.h"
// RapidFit Dalitz Plot Libraries
#include "DPBWResonanceShape.hh"
#include "DPFlatteShape.hh"
#include "DPBarrierL0.hh"
#include "DPBarrierL1.hh"
#include "DPBarrierL2.hh"
// Custom Libraries
#include "SphericalHarmonic.h"
using std::cout;
using std::endl;
// Constructor
Bs2PhiKKComponent::Bs2PhiKKComponent(int J2, double M2, double W2, string shape) : 
    _J1(1)        // Spin of phi --> fixed
  , _J2(J2)       // Spin of KK resonance
  , _M1(1019.461) // Mass of phi --> fixed
  , _M2(M2)       // Mass of KK resonance
  , _W1(4.266)    // Width of phi --> fixed
  , _W2(W2)       // Width of KK resonance
{
  _lambda_max = TMath::Min(_J1,_J2); // Maximum helicity
  for(int lambda = -_lambda_max; lambda <= +_lambda_max; lambda++)
  {
    _A.push_back(TComplex(sqrt(1.0/(1+2*_lambda_max)),0)); // Make our helicity amplitudes
  }
  // Breit Wigner
  if(shape=="BW")
  {
    _M = new DPBWResonanceShape(M2,W2,J2,493.677,493.677,1.7); // Typical values of barrier factor radius are 3 and 1.7 inverse GeV
  }
  // Flatte
  if(shape=="FT")
  {
    _M = new DPFlatteShape(M2, 199,139.570,139.570, 199*3,493.677,493.677); // Values for g0 and g1 are taken from Table 8 in LHCb-PAPER-2012-005
  }
}
// Get the corresponding helicity amplitude for a given value of helicity, instead of using array indices
TComplex* Bs2PhiKKComponent::A(int lambda)
{
  return &_A[lambda+_lambda_max];
}
// Mass-dependent part of the amplitude
TComplex Bs2PhiKKComponent::M(double m)
{
  retu
// Angular part of the amplitude
TComplex Bs2PhiKKComponent::F(double Phi, double theta1, double theta2)
{
  TComplex result(0, 0);
  for(int lambda = -_lambda_max; lambda <= +_lambda_max; lambda++)
  {
    result += *A(lambda) * Y(-lambda, _J1, TMath::Pi() - theta1, -Phi)*Y(lambda, _J2, theta2, 0);
  }
  return result;
}
// The full amplitude
TComplex Bs2PhiKKComponent::Amplitude(double m, double phi, double cos_theta1, double cos_theta2)
{
  return M(m)*F(phi, TMath::ACos(cos_theta1), TMath::ACos(cos_theta2));
}
// Set helicity amplitude parameters
void Bs2PhiKKComponent::SetHelicityAmplitudes(vector<TComplex> newA)
{
  if(newA.size()!=_A.size())
  {
    cout << newA.size() << " amplitudes given, but " << _A.size() << " required!" << endl;
  }
  for(unsigned int i = 0; i < _A.size(); i++)
  {
    _A[i] = newA[i];
  }
}rn _M->massShape(m);
}
