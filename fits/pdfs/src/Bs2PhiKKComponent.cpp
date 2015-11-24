#include "TMath.h"
#include "DPMassShape.hh"
#include "DPBWResonanceShape.hh"
#include "DPFlatteShape.hh"
#include "Bs2PhiKKComponent.h"
#include "DPBarrierFactor.hh"
#include "DPBarrierL0.hh"
#include "DPBarrierL1.hh"
#include "DPBarrierL2.hh"
#include "SphericalHarmonic.h"
#include <iostream>
using std::cout;
using std::endl;
Bs2PhiKKComponent::Bs2PhiKKComponent(int J2, double M2, double W2, string shape) : 
    _J1(1)        // Spin of phi --> fixed
  , _J2(J2)       // Spin of particle 2
  , _M1(1019.461) // Mass of phi --> fixed
  , _M2(M2)       // Mass of particle 2
  , _W1(4.266)    // Width of phi --> fixed
  , _W2(W2)       // Width of particle 2
{
  _lambda_max = TMath::Min(_J1,_J2);
  for(int lambda = -_lambda_max; lambda <= +_lambda_max; lambda++)
  {
    _A.push_back(TComplex(sqrt(1.0/(1+2*_lambda_max)),0)); // Make our helicity amplitudes
  }
  if(shape=="BW")
  {
    _M = new DPBWResonanceShape(M2,W2,J2,493.677,493.677,1.7); // Typical values of barrier factor radius are 3 and 1.7 inverse GeV
  }
  if(shape=="FT")
  {
    _M = new DPFlatteShape(M2, 199,139.570,139.570, 199*3,493.677,493.677);
  }
}
TComplex* Bs2PhiKKComponent::A(int lambda)
{
  return &_A[lambda+_lambda_max]; // Return the corresponding helicity amplitude given a value of helicity
}
TComplex Bs2PhiKKComponent::F(double Phi, double theta1, double theta2)
{
  TComplex result(0, 0);
  for(int lambda = -_lambda_max; lambda <= +_lambda_max; lambda++)
  {
    result += *A(lambda) * Y(-lambda, _J1, TMath::Pi() - theta1, -Phi)*Y(lambda, _J2, theta2, 0);
  }
  return result;
}
TComplex Bs2PhiKKComponent::Amplitude(double m, double phi, double cos_theta1, double cos_theta2)
{
  return M(m)*F(phi, TMath::ACos(cos_theta1), TMath::ACos(cos_theta2));
}
void Bs2PhiKKComponent::SetParameters(vector<TComplex> newA)
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
TComplex Bs2PhiKKComponent::M(double m)
{
  return _M->massShape(m);
}
