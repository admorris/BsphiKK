#ifndef __LEGENDREMOMENTSHAPE_H__
#define __LEGENDREMOMENTSHAPE_H__
#include <vector>
#include <string>
#include "TFile.h"
#include "TTree.h"
using std::string;
using std::vector;
class LegendreMomentShape
{
  public:
    LegendreMomentShape(string);
    LegendreMomentShape(const LegendreMomentShape&);
    ~LegendreMomentShape();
    double Evaluate(double,double,double,double);
    double Integral(double,double,double,double,double,double,double,double);
    double mKK_min;
    double mKK_max;
  protected:
    struct coefficient
    {
      int l,i,j,k;
      double val;
    };
    vector<coefficient> coeffs;
    bool init;
  private:
    string filename;
    double**** c;
    void createcoefficients();
    void storecoefficients();
    int l_max;
    int i_max;
    int k_max;
    int j_max;
    TTree* tree;
    TFile* file;
    bool copied;
    inline double int_legendre_sphPlm(int,int,double);
    inline int factorial(int n){ return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n; }
};
#endif
