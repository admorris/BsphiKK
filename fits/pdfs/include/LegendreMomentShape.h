#ifndef __LEGENDREMOMENTSHAPE_H__
#define __LEGENDREMOMENTSHAPE_H__
#include <string>
class LegendreMomentShape
{
  public:
    LegendreMomentShape(string filename = "LegendreMoments.root");
    LegendreMomentShape(const LegendreMomentShape&);
    ~LegendreMomentShape();
    double Evaluate(double,double,double,double);
    double mKK_min;
    double mKK_max;
  protected:
    double**** c;
    void createcoefficients();
    int l_max;
    int i_max;
    int k_max;
    int j_max;
};
#endif
