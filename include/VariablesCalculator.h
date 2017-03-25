/* Taken from Urania/Phys/LbTo4H/analysis/src
 * Author: Andrea Merli, September 2014
 */
#pragma once

// ROOT
#include "TLorentzVector.h"

class VariablesCalculator
{
 public:
  // Constructors
  VariablesCalculator();
  // Destructor
  ~VariablesCalculator();
  // Methods
  TLorentzVector MoveToFrame(TLorentzVector p, TLorentzVector pFrame);
  double CT(TLorentzVector p1, TLorentzVector p2, TLorentzVector p3);
  double PHI(TLorentzVector p1, TLorentzVector p2, TLorentzVector p3, TLorentzVector p4);
  double HelicityAngle(TLorentzVector p1, TLorentzVector p2);
 private:
};

