#include "HelicityAngleCalculator.h"

HelicityAngleCalculator::HelicityAngleCalculator(TLorentzVector a, TLorentzVector b, TLorentzVector c, TLorentzVector d)
{
  hP[0] = a;
  hP[1] = b;
  hP[2] = c;
  hP[3] = d;
  calculate();
}

void HelicityAngleCalculator::SetTracks(TLorentzVector a, TLorentzVector b, TLorentzVector c, TLorentzVector d)
{
  hP[0] = a;
  hP[1] = b;
  hP[2] = c;
  hP[3] = d;
  calculate();
}

void HelicityAngleCalculator::calculate()
{
/*******************************************************************************
    See page 12 of LHCb-ANA-2012-067. Replace muons with the resonant kaons.
    variables are named as if the decay is B0->d0d0->h-h+h-h+
*******************************************************************************/
  TLorentzVector BP = hP[0] + hP[1] + hP[2] + hP[3]; // B 4-momentum in the lab frame
  TLorentzVector dP[] = {hP[0] + hP[1], hP[2] + hP[3]}; // each daughter 4-momentum in the lab frame
  TLorentzVector hP_Bframe[4]; // each track 4-momentum in the B frame
  TLorentzVector hP_dframe[4]; // each track 4-momentum in the frame of its immediate parent
  TLorentzVector hP_other_dframe[4]; // each track 4-momentum in the frame of parent of the other track pair
  // Loop over tracks
  for(int j = 0; j < 4; j++)
  {
    // Boost each track into the B frame
    hP_Bframe[j] = hP[j];
    hP_Bframe[j].Boost(-BP.BoostVector());
    // Boost each track into the frame of its immediate parent
    hP_dframe[j] = hP[j];
    hP_dframe[j].Boost(-dP[j/2].BoostVector());
    // Boost each track into the frame of the parent of the other track pair
    hP_other_dframe[j] = hP[j];
    hP_other_dframe[j].Boost(-dP[(j/2+1)%2].BoostVector());
  }
  // Loop over daughters
  TVector3 dframe_e[2];
  TVector3 dframe_n[2];
  for(int j = 0; j < 2; j++)
  {
    int minus = 2*j;   // 0 and 2
    int plus  = 2*j+1; // 1 and 3
    int othminus = 2*((j+1)%2);
    int othplus = 2*((j+1)%2)+1;s
    dframe_e[j] = -1.0 * ((hP_other_dframe[othminus].Vect() + hP_other_dframe[othplus].Vect()) * (1.0/(hP_other_dframe[othminus].Vect() + hP_other_dframe[othplus].Vect()).Mag()));
    costheta[j] = (hP_dframe[plus].Vect() * (1.0/hP_dframe[plus].Vect().Mag())).Dot(dframe_e[j]);
    dframe_n[j] = (hP_Bframe[plus].Vect().Cross(hP_Bframe[minus].Vect())) * (1.0/(hP_Bframe[plus].Vect().Cross(hP_Bframe[minus].Vect())).Mag());
  }
  TVector3 Bframe_e = (hP_Bframe[0].Vect() + hP_Bframe[1].Vect()) * (1.0/(hP_Bframe[0].Vect() + hP_Bframe[1].Vect()).Mag());
  double cos_Phi = dframe_n[1].Dot(dframe_n[0]);
  double sin_Phi = (dframe_n[1].Cross(dframe_n[0])).Dot(Bframe_e);
  phi  = TMath::ACos(cos_Phi) * (sin_Phi/TMath::Abs(sin_Phi));
}
