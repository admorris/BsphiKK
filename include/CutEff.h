#ifndef __CUTEFF_H__
#define __CUTEFF_H__
#include <string>
#include <cmath>
using namespace std;
struct Cut_t
{
  Cut_t(string, string);
  string name;
  string cut;
  double eff = 0;
  double efferr = 0;
};
struct CutResult_t
{
  CutResult_t(double, double);
  double before;
  double after;
  double GetEff(){return after/before;}
  double GetEffErr()
  {
    double eff = GetEff();
    return sqrt((1-eff)*eff/before);
  }
};
CutResult_t CutEff(string, string, string, string);
CutResult_t CutEff(string, string, string, string, string);
#endif
