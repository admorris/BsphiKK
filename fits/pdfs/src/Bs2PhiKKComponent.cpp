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
// Constructor
Bs2PhiKKComponent::Bs2PhiKKComponent(int J2, double M2, double W2, string shape, double RBs, double RKK) : 
    _J1(1)        // Spin of phi --> fixed
  , _J2(J2)       // Spin of KK resonance
  , _M1(mphi) // Mass of phi --> fixed
  , _M2(M2)       // Mass of KK resonance
  , _W1(4.266)    // Width of phi --> fixed
  , _W2(W2)       // Width of KK resonance
  // the string shape chooses a resonance shape
  // RBs and RKK are barrier factor radii for the Bs and the KK resonance
{
  _lambda_max = TMath::Min(_J1,_J2); // Maximum helicity
  for(int lambda = -_lambda_max; lambda <= +_lambda_max; lambda++)
  {
    _A.push_back(TComplex(sqrt(1.0/(1+2*_lambda_max)),0)); // Make our helicity amplitudes
  }
  // Breit Wigner
  if(shape=="BW")
  {
    _M = new DPBWResonanceShape(M2,W2,J2,mK,mK,RKK); 
  }
  // Flatte
  if(shape=="FT")
  {
    _M = new DPFlatteShape(M2, 199,139.570,139.570, 199*3,mK,mK); // Values for g0 and g1 are taken from Table 8 in LHCb-PAPER-2012-005
  }
  Bsbarrier = new DPBarrierL0(RBs);
  switch (_J2)
  {
    case 0: KKbarrier = new DPBarrierL0(RKK);
            break;
    case 1: KKbarrier = new DPBarrierL1(RKK);
            break;
    case 2: KKbarrier = new DPBarrierL2(RKK);
            break;
    default: cout << "WARNING: Do not know which barrier factor to use." << endl;
             KKbarrier = new DPBarrierL0(RKK);
             break;
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
  return _M->massShape(m);
}
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
TComplex Bs2PhiKKComponent::Amplitude(double mKK, double phi, double cos_theta1, double cos_theta2)
{
  // Mass-dependent part
  double massPart       = M(mKK);
  // Angular part
  double angularPart    = F(phi, TMath::ACos(cos_theta1), TMath::ACos(cos_theta2));
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
}

