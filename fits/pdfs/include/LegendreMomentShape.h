#ifndef __LEGENDREMOMENTSHAPE_H__
#define __LEGENDREMOMENTSHAPE_H__
#include <string>
#include "TFile.h"
#include "TTree.h"
using std::string;
class LegendreMomentShape
{
  public:
    LegendreMomentShape(string);
    LegendreMomentShape(const LegendreMomentShape&);
    ~LegendreMomentShape();
    double Evaluate(double,double,double,double);
    double mKK_min;
    double mKK_max;
  protected:
    double**** c;
    void createcoefficients();
    void copycoefficients(double**** c);
    int l_max;
    int i_max;
    int k_max;
    int j_max;
    string filename;
    bool init;
  private:
    TTree* tree;
    TFile* file;
};
#endif
