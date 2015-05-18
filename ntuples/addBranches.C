#include <string.h>
#include <time.h>
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
#include "../progbar.h"
using namespace std;
Float_t safeLog(Double_t x)
{
  // Keep numbers in a sensible range for the sake of MVA training
  if(x>TMath::Exp(-25)) return TMath::Log(x);
  else return -25;
}
Float_t minOfFour(Double_t a, Double_t b, Double_t c, Double_t d)
{
  return TMath::Min(TMath::Min(a,b),TMath::Min(c,d));
}
void addBranches(string filename = "BsphiKK_data")
{
  const Int_t t0 = time(0);
  cout << "Adding branches to " << filename << endl;
/*Input************************************************************************/
  // Open the input file and create the output file
  TFile* infile  = new TFile((filename+"_cuts.root"   ).c_str()),
       * outfile = new TFile((filename+"_bdtVars.root").c_str(),"RECREATE");
  // Get the input tree and create an empty output tree
  TTree* intree  = (TTree*)infile->Get("DecayTree"),
       * outtree = intree->CloneTree(0);
  // Read the number of events in the input file
  Int_t n = intree->GetEntries();
/*PDG masses*******************************************************************/
  Double_t Kmass = 493.667, pimass = 139.570, pmass = 938.272;
/*Variables for new branches***************************************************/
  // Integers for mass hypothesis decisions (0-3: correspond to track indices)
  Int_t pion, proton, kaon;
  // Track 4-momentum
  TLorentzVector hP[4], pionP, protonP;
  // B and daughter 4-momentum
  TLorentzVector BP, dP[4];
/*Input branches***************************************************************/
  // These ones are used in creating the MVA discriminating variables
  Double_t B_s0_FDCHI2_OWNPV;   intree->SetBranchAddress("B_s0_FDCHI2_OWNPV",  &B_s0_FDCHI2_OWNPV  );
  Double_t B_s0_IPCHI2_OWNPV;   intree->SetBranchAddress("B_s0_IPCHI2_OWNPV",  &B_s0_IPCHI2_OWNPV  );
  Double_t B_s0_ENDVERTEX_CHI2; intree->SetBranchAddress("B_s0_ENDVERTEX_CHI2",&B_s0_ENDVERTEX_CHI2);
  Double_t B_s0_P;              intree->SetBranchAddress("B_s0_P",             &B_s0_P             );
  Double_t B_s0_PT;             intree->SetBranchAddress("B_s0_PT",            &B_s0_PT            );
/*PX Branches******************************************************************/
  // Track indices go in alphabetical order (minus before plus)
  // Note that LOKI uses a different order (plus before minus)
  // Do not to change this without careful scrutiny of what is happening with creating the new branches
  Double_t h_PX[4];
  intree->SetBranchAddress("B_s0_PX_kaon1",&h_PX[0]);
  intree->SetBranchAddress("B_s0_PX_kaon0",&h_PX[1]);
  intree->SetBranchAddress("B_s0_PX_kaon3",&h_PX[2]);
  intree->SetBranchAddress("B_s0_PX_kaon2",&h_PX[3]);
/*PY Branches*************************************************************/
  Double_t h_PY[4];
  intree->SetBranchAddress("B_s0_PY_kaon1",&h_PY[0]);
  intree->SetBranchAddress("B_s0_PY_kaon0",&h_PY[1]);
  intree->SetBranchAddress("B_s0_PY_kaon3",&h_PY[2]);
  intree->SetBranchAddress("B_s0_PY_kaon2",&h_PY[3]);
/*PZ Branches*************************************************************/
  Double_t h_PZ[4];
  intree->SetBranchAddress("B_s0_PZ_kaon1",&h_PZ[0]);
  intree->SetBranchAddress("B_s0_PZ_kaon0",&h_PZ[1]);
  intree->SetBranchAddress("B_s0_PZ_kaon3",&h_PZ[2]);
  intree->SetBranchAddress("B_s0_PZ_kaon2",&h_PZ[3]);
/*BCON PX Branches*************************************************************/
  Double_t h_BCON_PX[4];
  intree->SetBranchAddress("B_s0_BCON_PX_kaon1",&h_BCON_PX[0]);
  intree->SetBranchAddress("B_s0_BCON_PX_kaon0",&h_BCON_PX[1]);
  intree->SetBranchAddress("B_s0_BCON_PX_kaon3",&h_BCON_PX[2]);
  intree->SetBranchAddress("B_s0_BCON_PX_kaon2",&h_BCON_PX[3]);
/*BCON PY Branches*************************************************************/
  Double_t h_BCON_PY[4];
  intree->SetBranchAddress("B_s0_BCON_PY_kaon1",&h_BCON_PY[0]);
  intree->SetBranchAddress("B_s0_BCON_PY_kaon0",&h_BCON_PY[1]);
  intree->SetBranchAddress("B_s0_BCON_PY_kaon3",&h_BCON_PY[2]);
  intree->SetBranchAddress("B_s0_BCON_PY_kaon2",&h_BCON_PY[3]);
/*BCON PZ Branches*************************************************************/
  Double_t h_BCON_PZ[4];
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon1",&h_BCON_PZ[0]);
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon0",&h_BCON_PZ[1]);
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon3",&h_BCON_PZ[2]);
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon2",&h_BCON_PZ[3]);
/*ProbNNp Branches*************************************************************/
  Double_t h_ProbNNp[4];
  intree->SetBranchAddress("Kminus_ProbNNp", &h_ProbNNp[0]);
  intree->SetBranchAddress("Kplus_ProbNNp",  &h_ProbNNp[1]);
  intree->SetBranchAddress("Kminus0_ProbNNp",&h_ProbNNp[2]);
  intree->SetBranchAddress("Kplus0_ProbNNp", &h_ProbNNp[3]);
/*ProbNNk Branches*************************************************************/
  Double_t h_ProbNNk[4];
  intree->SetBranchAddress("Kminus_ProbNNk", &h_ProbNNk[0]);
  intree->SetBranchAddress("Kplus_ProbNNk",  &h_ProbNNk[1]);
  intree->SetBranchAddress("Kminus0_ProbNNk",&h_ProbNNk[2]);
  intree->SetBranchAddress("Kplus0_ProbNNk", &h_ProbNNk[3]);
/*ProbNNpi Branches************************************************************/
  Double_t h_ProbNNpi[4];
  intree->SetBranchAddress("Kminus_ProbNNpi", &h_ProbNNpi[0]);
  intree->SetBranchAddress("Kplus_ProbNNpi",  &h_ProbNNpi[1]);
  intree->SetBranchAddress("Kminus0_ProbNNpi",&h_ProbNNpi[2]);
  intree->SetBranchAddress("Kplus0_ProbNNpi", &h_ProbNNpi[3]);
/*IPCHI2_OWNPV Branches************************************************************/
  Double_t h_IPCHI2_OWNPV[4];
  intree->SetBranchAddress("Kminus_IPCHI2_OWNPV", &h_IPCHI2_OWNPV[0]);
  intree->SetBranchAddress("Kplus_IPCHI2_OWNPV",  &h_IPCHI2_OWNPV[1]);
  intree->SetBranchAddress("Kminus0_IPCHI2_OWNPV",&h_IPCHI2_OWNPV[2]);
  intree->SetBranchAddress("Kplus0_IPCHI2_OWNPV", &h_IPCHI2_OWNPV[3]);
/*BDT branches*****************************************************************/
  Float_t B_s0_ln_FDCHI2; outtree->Branch("B_s0_ln_FDCHI2", &B_s0_ln_FDCHI2, "B_s0_ln_FDCHI2/F" );
  Float_t B_s0_ln_IPCHI2; outtree->Branch("B_s0_ln_IPCHI2", &B_s0_ln_IPCHI2, "B_s0_ln_IPCHI2/F" );
  Float_t B_s0_ln_EVCHI2; outtree->Branch("B_s0_ln_EVCHI2", &B_s0_ln_EVCHI2, "B_s0_ln_EVCHI2/F" );
  Float_t B_s0_PT_fiveGeV;outtree->Branch("B_s0_PT_fiveGeV",&B_s0_PT_fiveGeV,"B_s0_PT_fiveGeV/F");
  Float_t B_s0_Eta;       outtree->Branch("B_s0_Eta",       &B_s0_Eta,       "B_s0_Eta/F"       );
  Float_t minK_ln_IPCHI2; outtree->Branch("minK_ln_IPCHI2", &minK_ln_IPCHI2, "minK_ln_IPCHI2/F" );
  Float_t minK_PT_GeV;    outtree->Branch("minK_PT_GeV",    &minK_PT_GeV,    "minK_PT_GeV/F"    );
  Double_t h_PT[4]; // Necessary for min Kaon PT
/*New mass branches************************************************************/
  TLorentzVector BCON_KK_P;   Double_t BCON_KK_M;   outtree->Branch("BCON_KK_M",  &BCON_KK_M,  "BCON_KK_M/D"  );
  TLorentzVector phiKplusP;   Double_t phiKplusM;   outtree->Branch("phiKplusM",  &phiKplusM,  "phiKplusM/D"  );
  TLorentzVector phiKminusP;  Double_t phiKminusM;  outtree->Branch("phiKminusM", &phiKminusM, "phiKminusM/D" );
  TLorentzVector phipiplusP;  Double_t phipiplusM;  outtree->Branch("phipiplusM", &phipiplusM, "phipiplusM/D" );
  TLorentzVector phipiminusP; Double_t phipiminusM; outtree->Branch("phipiminusM",&phipiminusM,"phipiminusM/D");
  TLorentzVector phiKpiP;     Double_t phiKpiM;     outtree->Branch("phiKpiM",    &phiKpiM,    "phiKpiM/D"    );
  TLorentzVector phipP;       Double_t phipM;       outtree->Branch("phipM",      &phipM,      "phipM/D"      );
  TLorentzVector phipbarP;    Double_t phipbarM;    outtree->Branch("phipbarM",   &phipbarM,   "phipbarM/D"   );
  TLorentzVector phiKpP;      Double_t phiKpM;      outtree->Branch("phiKpM",     &phiKpM,     "phiKpM/D"     );
/*Helicity angle branches******************************************************/
  // Track 4-momentum in B frame and daughter frames
  TLorentzVector Bframe_h_P[4], Bframe_d_P[4], dframe_h_P[4];
  TVector3 CrossProduct[2];
  Double_t Phi_angle; outtree->Branch("Phi_angle", &Phi_angle, "Phi_angle/D" );
  Double_t cos_theta[2];
  outtree->Branch("cos_theta1",&cos_theta[0],"cos_theta1/D");
  outtree->Branch("cos_theta2",&cos_theta[1],"cos_theta2/D");
/*Event loop*******************************************************************/
  for(Int_t i = 0; i < n; i++)
  {
    intree->GetEntry(i);
/*BDT variables****************************************************************/
    B_s0_ln_FDCHI2  = safeLog(B_s0_FDCHI2_OWNPV);
    B_s0_ln_IPCHI2  = safeLog(B_s0_IPCHI2_OWNPV);
    B_s0_ln_EVCHI2  = safeLog(B_s0_ENDVERTEX_CHI2/5.0); // 5 degrees of freedom
    B_s0_PT_fiveGeV = B_s0_PT/5000; // Change units for sensible range of numbers
    B_s0_Eta        = TMath::ACosH(B_s0_P/B_s0_PT);
    minK_ln_IPCHI2  = safeLog(minOfFour(h_IPCHI2_OWNPV[0],h_IPCHI2_OWNPV[1],h_IPCHI2_OWNPV[2],h_IPCHI2_OWNPV[3]));
    for(Int_t j = 0; j < 4; j++) h_PT[j] = TMath::Sqrt(TMath::Power(h_PX[j],2)+TMath::Power(h_PY[j],2));
    minK_PT_GeV     = minOfFour(h_PT[0],h_PT[1],h_PT[2],h_PT[3])/1000;
/*Mass branches****************************************************************/
    // Track 4-momenta with constrained Bs mass
    for(Int_t j = 0; j < 4; j++) hP[j].SetXYZM(h_BCON_PX[j],h_BCON_PY[j],h_BCON_PZ[j],Kmass);
    BCON_KK_P = hP[2] + hP[3];
    BCON_KK_M = BCON_KK_P.M();
    // Initial 4K hypothesis
    for(Int_t j = 0; j < 4; j++) hP[j].SetXYZM(h_PX[j],h_PY[j],h_PZ[j],Kmass);
    // Reconstruct B and daughter 4-momentum
    BP    = hP[0] + hP[1] + hP[2] + hP[3];
    dP[0] = hP[0] + hP[1];
    dP[1] = hP[2] + hP[3];
    // Both phiK+− branches
    phiKminusP = hP[0] + hP[1] + hP[2];
    phiKminusM = phiKminusP.M();
    phiKplusP = hP[0] + hP[1] + hP[3];
    phiKplusM = phiKplusP.M();
/*phi K* and open charm backgrounds********************************************/
    // Decide which is the pion
    if(h_ProbNNpi[2] > h_ProbNNpi[3])
    {
      // first is the pion
      pion = 2;
      kaon = 3;
    }
    else
    {
      // second is the pion
      pion = 3;
      kaon = 2;
    }
    // phi K pi
    pionP.SetXYZM(h_PX[pion],h_PY[pion],h_PZ[pion],pimass);
    phiKpiP = hP[0] + hP[1] + pionP + hP[kaon];
    phiKpiM = phiKpiP.M();
    // phi pi-
    pionP.SetXYZM(h_PX[2],h_PY[2],h_PZ[2],pimass);
    phipiminusP = hP[0] + hP[1] + pionP;
    phipiminusM = phipiminusP.M();
    // phi pi+
    pionP.SetXYZM(h_PX[3],h_PY[3],h_PZ[3],pimass);
    phipiplusP = hP[0] + hP[1] + pionP;
    phipiplusM = phipiplusP.M();
/*Lambda backgrounds***********************************************************/
    // Decide which is the proton
    if(h_ProbNNp[2] > h_ProbNNp[3])
    {
      // first is the proton
      proton = 2;
      kaon   = 3;
    }
    else
    {
      // second is the proton
      proton = 3;
      kaon   = 2;
    }
    // phi K proton
    protonP.SetXYZM(h_PX[proton],h_PY[proton],h_PZ[proton],pmass);
    phiKpP = hP[0] + hP[1] + protonP + hP[kaon];
    phiKpM = phiKpP.M();
    // phi proton
    protonP.SetXYZM(h_PX[3],h_PY[3],h_PZ[3],pmass);
    phipP = hP[0] + hP[1] + protonP;
    phipM = phipP.M();
    // phi anti-proton
    protonP.SetXYZM(h_PX[2],h_PY[2],h_PZ[2],pmass);
    phipbarP = hP[0] + hP[1] + protonP;
    phipbarM = phipbarP.M();
/*Helicity angles**************************************************************/
    // Loop over Kaons
    for(Int_t j = 0; j < 4; j++)
    {
      // Boost into B frame
      Bframe_h_P[j] = hP[j];
      Bframe_h_P[j].Boost(BP.BoostVector());
      // Boost into daughter frames
      dframe_h_P[j] = hP[j];
      dframe_h_P[j].Boost(dP[j/2].BoostVector());
    }
    // Loop over daughters
    for(Int_t j = 0; j < 2; j++)
    {
      // Boost daughters into B frame
      Bframe_d_P[j] = dP[j];
      Bframe_d_P[j].Boost(BP.BoostVector());
      // theta_i is the angle between the Kplus from daughter_i in the daughter frame and the daughter in the B frame
//      cos_theta[j] = TMath::Cos(dframe_h_P[2*j+1].Vect().Angle(Bframe_d_P[j].Vect()));
      cos_theta[j] = Bframe_d_P[j].Vect().Dot(dframe_h_P[2*j+1].Vect())/(Bframe_d_P[j].Vect().Mag()*dframe_h_P[2*j+1].Vect().Mag());
      // Calculate cross product of K pairs in B frame (vector normal to decay plane)
      CrossProduct[j] = Bframe_h_P[2*j].Vect().Cross(Bframe_h_P[2*j+1].Vect());
    }
    // Phi is the inverse cos of the normalised dot product of the two above cross products
    Phi_angle = TMath::ACos(CrossProduct[0].Dot(CrossProduct[1])/(CrossProduct[0].Mag()*CrossProduct[1].Mag()));
/*Fill tree and show progress**************************************************/
    outtree->Fill();
    if(i%100 == 0)
    {
      progbar(i,n);
      timestamp(t0);
    }
  }
  cout << endl; // New line to finish off the progress bar
/*Write the output**************************************************************/
  outtree->Write();
  infile->Close();
  outfile->Close();
  return;
}
