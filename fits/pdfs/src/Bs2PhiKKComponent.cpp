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
#include "DPHelpers.hh"
// Custom Libraries
#include "SphericalHarmonic.h"
using std::cout;
using std::endl;
using std::flush;
// Constructor
Bs2PhiKKComponent::Bs2PhiKKComponent(int J2, double M2, double W2, string shape, double RBs, double RKK) : 
    _J1(1)        // Spin of phi --> fixed
  , _J2(J2)       // Spin of KK resonance
  , _M1(mphi)     // Mass of phi --> fixed
  , _M2(M2)       // Mass of KK resonance
  , _W1(4.266)    // Width of phi --> fixed
  , _W2(W2)       // Width of KK resonance
  , _shape(shape) // chooses a resonance shape
  , _RBs(RBs)     // RBs and RKK are barrier factor radii for the Bs and the KK resonance
  , _RKK(RKK)
{
  Initialise();
}
void Bs2PhiKKComponent::Initialise()
{
  _lambda_max = TMath::Min(_J1,_J2); // Maximum helicity
  int n = 1+2*_lambda_max;
  _Amag   = new double[n];
  _Aphase = new double[n];
  for(int lambda = -_lambda_max; lambda <= +_lambda_max; lambda++)
  {
    int i = lambda+_lambda_max;
    _Amag[i] = sqrt(1.0/(n));
    _Aphase[i] = 0;
  }
  // Breit Wigner
  if(_shape=="BW")
  {
    _M = new DPBWResonanceShape(_M2,_W2,_J2,mK,mK,_RKK); 
  }
  // Flatte
  if(_shape=="FT")
  {
    _M = new DPFlatteShape(_M2, 199,139.570,139.570, 199*3,mK,mK); // Values for g0 and g1 are taken from Table 8 in LHCb-PAPER-2012-005
  }
  Bsbarrier = new DPBarrierL0(_RBs);
  switch (_J2)
  {
    case 0: KKbarrier = new DPBarrierL0(_RKK);
            break;
    case 1: KKbarrier = new DPBarrierL1(_RKK);
            break;
    case 2: KKbarrier = new DPBarrierL2(_RKK);
            break;
    default: cout << "WARNING: Do not know which barrier factor to use." << endl;
             KKbarrier = new DPBarrierL0(_RKK);
             break;
  }
}
Bs2PhiKKComponent::Bs2PhiKKComponent(const Bs2PhiKKComponent& copy) : 
    _J2(copy._J2)
  , _M2(copy._M2)
  , _W2(copy._W2)
  , _shape(copy._shape)
  , _RBs(copy._RBs)
  , _RKK(copy._RKK)
{
  Initialise();
  for(int lambda = -_lambda_max; lambda <= _lambda_max; lambda++)
  {
    int i = lambda+_lambda_max;
    _Amag[i] = copy._Amag[i];
    _Aphase[i] = copy._Aphase[i];
  }
}
Bs2PhiKKComponent::~Bs2PhiKKComponent()
{
  delete[] _Amag;
  delete[] _Aphase;
  delete _M;
  delete Bsbarrier;
  delete KKbarrier;
}
// Get the corresponding helicity amplitude for a given value of helicity, instead of using array indices
TComplex Bs2PhiKKComponent::A(int lambda)
{
  int i = lambda + _lambda_max;
  return TComplex(_Amag[i],_Aphase[i],true);
}
// Mass-dependent part of the amplitude
TComplex Bs2PhiKKComponent::M(double m)
{
  return _M->massShape(m);
}
// Angular part of the amplitude
TComplex Bs2PhiKKComponent::F(double Phi, double cos_theta1, double cos_theta2)
{
  TComplex result(0, 0);
  for(int lambda = -_lambda_max; lambda <= _lambda_max; lambda++)
  {
    result += A(lambda) * Y(_J1, -lambda, -cos_theta1, -Phi)*Y(_J2, lambda, cos_theta2, 0);
  }
  return result;
}
// The full amplitude
TComplex Bs2PhiKKComponent::Amplitude(double mKK, double phi, double cos_theta1, double cos_theta2)
{
  // Mass-dependent part
  double massPart       = M(mKK);
  // Angular part
  double angularPart    = F(phi, cos_theta1, cos_theta2);
  // Orbital factor
  // Masses
  double m_min = mK + mK;
  double m_max = mBs - _M1;
  double m0_eff = m_min + (m_max - m_min)*(1+TMath::TanH( (_M2 - (m_min+m_max)/2.)/(m_max - m_min)))/2;
  // Momenta
  double pBs  = DPHelpers::daughterMomentum(mBs, _M1, mKK   );
  double pKK  = DPHelpers::daughterMomentum(mKK, mK , mK    );
  double pBs0 = DPHelpers::daughterMomentum(mBs, _M1, m0_eff);
  double pKK0 = DPHelpers::daughterMomentum(_M2, mK , mK    ); 
  double orbitalFactor = TMath::Power(pBs/mBs,   0)*
                         TMath::Power(pKK/mKK, _J2);
  // Barrier factors
  double barrierFactor = Bsbarrier->barrier( pBs0, pBs )*
                         KKbarrier->barrier( pKK0, pKK );
  // Result
  return massPart * angularPart * orbitalFactor * barrierFactor;
}
// Set helicity amplitude parameters
void Bs2PhiKKComponent::SetHelicityAmplitudes(int i, double mag, double phase)
{
  _Amag[i] = mag;
  _Aphase[i] = phase;
}
void Bs2PhiKKComponent::Print()
{
  cout << "| Spin-" << _J2 << " KK resonance" << endl;
  cout << "| Mass:    \t" << _M2 << " MeV" << endl;
  cout << "| Width:   \t" << _W2 << " MeV" << endl;
  cout << "| Helicity:\t";
  for(int lambda = -_lambda_max; lambda <= +_lambda_max; lambda++)
  {
    cout << lambda << " ";
  }
  cout << endl;
}
