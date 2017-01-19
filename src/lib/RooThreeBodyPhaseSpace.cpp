#include "RooThreeBodyPhaseSpace.h"

#include "RooFit.h"
#include "RooAbsReal.h"

#include <cmath>

//ClassImp(RooThreeBodyPhaseSpace)

RooThreeBodyPhaseSpace::RooThreeBodyPhaseSpace(const char* name, const char* title, RooAbsReal& _m12, RooAbsReal& _M, RooAbsReal& _m1, RooAbsReal& _m2, RooAbsReal& _m3) :
  RooAbsPdf(name, title),
  m12("m12","Dependent",this,_m12),
  M("M","Parent mass",this,_M),
  m1("m1","Body 1 mass",this,_m1),
  m2("m2","Body 2 mass",this,_m2),
  m3("m3","Body 3 mass",this,_m3)
{
}

RooThreeBodyPhaseSpace::RooThreeBodyPhaseSpace(const RooThreeBodyPhaseSpace& other, const char* name) :
  RooAbsPdf(other,name),
  m12("m12",this,other.m12),
  M("M",this,other.M),
  m1("m1",this,other.m1),
  m2("m2",this,other.m2),
  m3("m3",this,other.m3)
{
}

Double_t RooThreeBodyPhaseSpace::evaluate() const
{
  return p(m12, m1, m2) * p(M, m12, m3);
}

Double_t RooThreeBodyPhaseSpace::p(Double_t m, Double_t ma, Double_t mb) const
{
  Double_t momentum;
  momentum=(pow(m,2)-pow(ma+mb,2))*(pow(m,2)-pow(ma-mb,2));
  if(momentum<0) return 0;
  momentum=sqrt(momentum);
  momentum/=2*m;
  return momentum;
}

