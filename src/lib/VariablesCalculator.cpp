/* Taken from Urania/Phys/LbTo4H/analysis/src
 * Author: Andrea Merli, September 2014
 */
#include "VariablesCalculator.h"

// STL
#include <math.h>
// ROOT
#include "TVector3.h"

// Constructors
VariablesCalculator::VariablesCalculator()
{
}

// Destructor
VariablesCalculator::~VariablesCalculator()
{
}

// Methods
TLorentzVector
VariablesCalculator::MoveToFrame(TLorentzVector p, TLorentzVector pFrame)
{
  // returns the momentum of the particle in the CM frame defined by pFrame
  TLorentzVector pCM = p;
  pCM.Boost( -pFrame.BoostVector() );
  return pCM;
}

double 
VariablesCalculator::CT(TLorentzVector p1, TLorentzVector p2, TLorentzVector p3)
{
  // calculates vec(p1) . [vec(p2) x vec(p3)]
  double ct = p1.Vect().Dot( p2.Vect().Cross(p3.Vect()) );
  return ct;
}

double 
VariablesCalculator::PHI(TLorentzVector p1, TLorentzVector p2, TLorentzVector p3, TLorentzVector p4)
{
  // calculates the angle between the plane defined by (p1,p2) and (p3,p4)
  TVector3 n12 = p1.Vect().Cross( p2.Vect() ).Unit();
  TVector3 n34 = p3.Vect().Cross( p4.Vect() ).Unit();
  TVector3 z = (p1+p2).Vect().Unit();
  double cosPhi = n12.Dot(n34);
  double sinPhi = n12.Cross(n34).Dot(z);
  double phi = atan2(sinPhi,cosPhi); //defined in [-pi,pi]
  return phi;
}

double 
VariablesCalculator::HelicityAngle(TLorentzVector p1, TLorentzVector p2)
{
  // The helicity angle is defined as the angle between one of the two momenta in the p1+p2 rest frame
  // respect to the momentum of the p1+p2 system in the decaying particle rest frame (ptot)
  TLorentzVector p12 = p1+p2;
  TLorentzVector pcm1= MoveToFrame(p1, p12);
  double cosHel = ( pcm1.Vect() ).Dot( p12.Vect() )/
    ( pcm1.Vect().Mag() * p12.Vect().Mag() );
  return cosHel;
}
