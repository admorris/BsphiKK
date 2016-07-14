#ifndef __CUTEFF_H__
#define __CUTEFF_H__
#include <string>
#include <cmath>
#include "TTree.h"
#include "TCanvas.h" // might want it to run faster
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
// Version that opens a file
CutResult_t CutEff(string, string, string);
// Version that reads a TTrre.
// Prefereable if this function is to be called many times for the same file
CutResult_t CutEff(TTree*, string, string);
#endif
