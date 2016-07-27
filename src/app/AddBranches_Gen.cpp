#include <string.h>
#include <iostream>
#include "TSystem.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TStyle.h"
#include "safeLog.h"
using namespace std;
void addBranches(string filename = "BsphiKK_data")
{
  cout << "Adding branches to " << filename << endl;
/*Input************************************************************************/
  // Open the input file and create the output file
  TFile* infile  = TFile::Open((filename+".root"   ).c_str()),
       * outfile = TFile::Open((filename+"_mvaVars.root").c_str(),"RECREATE");
  // Get the input tree and create an empty output tree
  TTree* intree  = (TTree*)infile->Get("MCDecayTreeTuple/MCDecayTree"),
       * outtree = intree->CloneTree(0);
  // Read the number of events in the input file
  Int_t n = intree->GetEntries();
/*PDG masses*******************************************************************/
  Double_t Kmass = 493.667;;
/*Variables for new branches***************************************************/
  // Track 4-momentum
  TLorentzVector hP[4], pionP, protonP;
  // B and daughter 4-momentum
  TLorentzVector BP, dP[4];
/*Input branches***************************************************************/
/*PX Branches******************************************************************/
  // Track indices go in alphabetical order (minus before plus)
  // Note that LOKI uses a different order (plus before minus)
  // Do not to change this without careful scrutiny of what is happening with creating the new branches
  Double_t h_PX[4];
  intree->SetBranchAddress("Kminus_TRUEP_X", &h_PX[0]);
  intree->SetBranchAddress("Kplus_TRUEP_X",  &h_PX[1]);
  intree->SetBranchAddress("Kminus0_TRUEP_X",&h_PX[2]);
  intree->SetBranchAddress("Kplus0_TRUEP_X", &h_PX[3]);
/*PY Branches******************************************************************/
  Double_t h_PY[4];
  intree->SetBranchAddress("Kminus_TRUEP_Y", &h_PY[0]);
  intree->SetBranchAddress("Kplus_TRUEP_Y",  &h_PY[1]);
  intree->SetBranchAddress("Kminus0_TRUEP_Y",&h_PY[2]);
  intree->SetBranchAddress("Kplus0_TRUEP_Y", &h_PY[3]);
/*PZ Branches******************************************************************/
  Double_t h_PZ[4];
  intree->SetBranchAddress("Kminus_TRUEP_Z", &h_PZ[0]);
  intree->SetBranchAddress("Kplus_TRUEP_Z",  &h_PZ[1]);
  intree->SetBranchAddress("Kminus0_TRUEP_Z",&h_PZ[2]);
  intree->SetBranchAddress("Kplus0_TRUEP_Z", &h_PZ[3]);
/*Eta branches*****************************************************************/
  Double_t B_s0_Eta; outtree->Branch("B_s0_Eta", &B_s0_Eta, "B_s0_Eta/D");
  Double_t Phi_Eta;  outtree->Branch("Phi_Eta",  &Phi_Eta,  "Phi_Eta/D" );
  Double_t KK_Eta;   outtree->Branch("KK_Eta",   &KK_Eta,   "KK_Eta/D"  );
  Double_t K_Eta[4];
  outtree->Branch("Kminus_Eta",  &K_Eta[0], "Kminus_Eta/D" );
  outtree->Branch("Kplus_Eta",   &K_Eta[1], "Kplus_Eta/D"  );
  outtree->Branch("Kminus0_Eta", &K_Eta[2], "Kminus0_Eta/D");
  outtree->Branch("Kplus0_Eta",  &K_Eta[3], "Kplus0_Eta/D" );
  Double_t K_Theta[4];
  outtree->Branch("Kminus_Theta",  &K_Theta[0], "Kminus_Theta/D" );
  outtree->Branch("Kplus_Theta",   &K_Theta[1], "Kplus_Theta/D"  );
  outtree->Branch("Kminus0_Theta", &K_Theta[2], "Kminus0_Theta/D");
  outtree->Branch("Kplus0_Theta",  &K_Theta[3], "Kplus0_Theta/D" );
/*Helicity angle branches******************************************************/
  // Track 4-momentum in B frame and daughter frames
  TLorentzVector Bframe_h_P[4], dframe_h_P[4], dframe_other_h_P[4];
  TVector3 Bframe_e, dframe_e[2], dframe_n[2];
  Double_t Phi_angle; outtree->Branch("Phi_angle", &Phi_angle, "Phi_angle/D" );
  Double_t sin_Phi; outtree->Branch("sin_Phi", &sin_Phi, "sin_Phi/D" );
  Double_t cos_Phi; outtree->Branch("cos_Phi", &cos_Phi, "cos_Phi/D" );
  Double_t KK_M; outtree->Branch("KK_M", &KK_M, "KK_M/D" );
  Double_t cos_theta[2];
  outtree->Branch("cos_theta1",&cos_theta[0],"cos_theta1/D");
  outtree->Branch("cos_theta2",&cos_theta[1],"cos_theta2/D");
/*Event loop*******************************************************************/
  for(Int_t i = 0; i < n; i++)
  {
    intree->GetEntry(i);
/*Mass branches****************************************************************/
    // Track 4-momenta with constrained Bs mass
    // Initial 4K hypothesis
    for(Int_t j = 0; j < 4; j++) hP[j].SetXYZM(h_PX[j],h_PY[j],h_PZ[j],Kmass);
    // Reconstruct B and daughter 4-momentum
    BP    = hP[0] + hP[1] + hP[2] + hP[3];
    dP[0] = hP[0] + hP[1];
    dP[1] = hP[2] + hP[3];
    KK_M = dP[1].M();
/*Pseudorapidity and polar angle***********************************************/
    B_s0_Eta = BP.Eta();
    Phi_Eta  = dP[0].Eta();
    KK_Eta   = dP[1].Eta();
    for(Int_t j = 0; j < 4; j++)
    {
      K_Eta[j] = hP[j].Eta();
      K_Theta[j] = hP[j].Theta();
    }
/*Helicity angles**************************************************************/
/*******************************************************************************
    See page 12 of LHCb-ANA-2012-067. Replace muons with the resonant kaons.
*******************************************************************************/
    // Loop over Kaons
    for(Int_t j = 0; j < 4; j++)
    {
      // Boost into B frame
      Bframe_h_P[j] = hP[j];
      Bframe_h_P[j].Boost(-BP.BoostVector());
      // Boost into daughter frames
      dframe_h_P[j] = hP[j];
      dframe_h_P[j].Boost(-dP[j/2].BoostVector());
      // Boost into "wrong" daughter frames
      // Thanks Dianne!
      dframe_other_h_P[j] = hP[j];
      dframe_other_h_P[j].Boost(-dP[(j-3)/-2].BoostVector());
    }
    // Loop over daughters
    for(Int_t j = 0; j < 2; j++)
    {
      Int_t minus = 2*j;   // 0 and 2
      Int_t plus  = 2*j+1; // 1 and 3
      dframe_e[j] = -1.0 * ((dframe_other_h_P[minus].Vect() + dframe_other_h_P[plus].Vect()) * (1.0/(dframe_other_h_P[minus].Vect() + dframe_other_h_P[plus].Vect()).Mag()));
      cos_theta[j] = (dframe_h_P[plus].Vect() * (1.0/dframe_h_P[plus].Vect().Mag())).Dot(dframe_e[j]);
      dframe_n[j] = (Bframe_h_P[plus].Vect().Cross(Bframe_h_P[minus].Vect())) * (1.0/(Bframe_h_P[plus].Vect().Cross(Bframe_h_P[minus].Vect())).Mag());
    }
    Bframe_e = (Bframe_h_P[0].Vect() + Bframe_h_P[1].Vect()) * (1.0/(Bframe_h_P[0].Vect() + Bframe_h_P[1].Vect()).Mag());
    cos_Phi = dframe_n[1].Dot(dframe_n[0]);
    sin_Phi = (dframe_n[1].Cross(dframe_n[0])).Dot(Bframe_e);
    Phi_angle  = TMath::ACos(cos_Phi) * (sin_Phi/TMath::Abs(sin_Phi));
/*Fill tree and show progress**************************************************/
    outtree->Fill();
  }
/*Write the output*************************************************************/
  outtree->Write();
  infile->Close();
  outfile->Close();
  return;
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    cout << "Please provide a filename." << endl;
    return 1;
  }
  else if(argc>2)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  addBranches((string)argv[1]);
  return 0;
}
