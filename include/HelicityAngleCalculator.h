#include "TLorentzVector.h"

class HelicityAngleCalculator
{
  public:
    HelicityAngleCalculator(TLorentzVector,TLorentzVector,TLorentzVector,TLorentzVector);
    double Phi() {return phi;}
    double CosTheta1() {return costheta[0];}
    double CosTheta2() {return costheta[1];}
  private:
    TLorentzVector hP[4];
    void SetTracks(TLorentzVector,TLorentzVector,TLorentzVector,TLorentzVector);
    void calculate();
    double phi;
    double costheta[2];
};
