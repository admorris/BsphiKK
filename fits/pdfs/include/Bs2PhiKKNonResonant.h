/** @class Bs2PhiKKNonResonant Bs2PhiKKNonResonant.cpp
 *
 *  RapidFit PDF for Bs2PhiKKNonResonant
 *
 *  @author Adam Morris
 *  @date Feb 2016
 */
#ifndef __BS2PHIKKNonResonant_H__
#define __BS2PHIKKNonResonant_H__
// ROOT
#include "TComplex.h"
// Std
#include <string>
#include <vector>
// RapidFit
#include "Bs2PhiKKComponent.h"
using std::string;
using std::vector;
class Bs2PhiKKNonResonant
{
  public:
    static double Evaluate(double);
};
#endif

