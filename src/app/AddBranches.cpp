// Std headers
#include <string.h>
#include <iostream>
#include <algorithm>
// ROOT headers
#include "TSystem.h"
#include "TCanvas.h"
#include "TComplex.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TStyle.h"
// Custom headers
#include "safeLog.h"
#include "GetTree.h"
using namespace std;
// Normal track indices go in alphabetical order (minus before plus)
// Note that LOKI uses a different order (plus before minus)
// Do not to change this without careful scrutiny of what is happening with creating the new branches
void addBranches(string inputfilename = "BsphiKK_data_cuts.root", string outputfilename = "BsphiKK_data_mvaVars.root")
{
  bool isMC = inputfilename.find("MC") != string::npos;
  cout << "Reading from " << inputfilename << endl;
/*Input************************************************************************/
  // Open the input file and create the output file
  TFile* infile  = TFile::Open(inputfilename.c_str()),
       * outfile = TFile::Open(outputfilename.c_str(),"RECREATE");
  // Get the input tree and create an empty output tree
  TTree* intree  = GetTree(infile);
  outfile->cd();
  TTree* outtree = intree->CloneTree(0);
  // Read the number of events in the input file
  int n = intree->GetEntries();
/*PDG masses*******************************************************************/
  double Kmass = 493.667, pimass = 139.570, pmass = 938.272;
/*Variables for new branches***************************************************/
  // Integers for mass hypothesis decisions (0-3: correspond to track indices)
  int pion, proton, kaon;
  // Track 4-momentum
  TLorentzVector hP[4], h_TRUEP[4], h_BCONP[4], pionP, protonP;
  // B and daughter 4-momentum
  TLorentzVector BP, dP[4];
/*Input branches***************************************************************/
  // These ones are used in creating the MVA discriminating variables
  double B_s0_FDCHI2_OWNPV;   intree->SetBranchAddress("B_s0_FDCHI2_OWNPV",  &B_s0_FDCHI2_OWNPV  );
  double B_s0_IPCHI2_OWNPV;   intree->SetBranchAddress("B_s0_IPCHI2_OWNPV",  &B_s0_IPCHI2_OWNPV  );
  double B_s0_ENDVERTEX_CHI2; intree->SetBranchAddress("B_s0_ENDVERTEX_CHI2",&B_s0_ENDVERTEX_CHI2);
  double B_s0_P;              intree->SetBranchAddress("B_s0_P",             &B_s0_P             );
  double B_s0_PT;             intree->SetBranchAddress("B_s0_PT",            &B_s0_PT            );
/*LOKI PX Branches*************************************************************/
  double h_LOKI_PX[4];
  intree->SetBranchAddress("B_s0_PX_kaon1",&h_LOKI_PX[0]);
  intree->SetBranchAddress("B_s0_PX_kaon0",&h_LOKI_PX[1]);
  intree->SetBranchAddress("B_s0_PX_kaon3",&h_LOKI_PX[2]);
  intree->SetBranchAddress("B_s0_PX_kaon2",&h_LOKI_PX[3]);
/*LOKI PY Branches*************************************************************/
  double h_LOKI_PY[4];
  intree->SetBranchAddress("B_s0_PY_kaon1",&h_LOKI_PY[0]);
  intree->SetBranchAddress("B_s0_PY_kaon0",&h_LOKI_PY[1]);
  intree->SetBranchAddress("B_s0_PY_kaon3",&h_LOKI_PY[2]);
  intree->SetBranchAddress("B_s0_PY_kaon2",&h_LOKI_PY[3]);
/*LOKI PZ Branches*************************************************************/
  double h_LOKI_PZ[4];
  intree->SetBranchAddress("B_s0_PZ_kaon1",&h_LOKI_PZ[0]);
  intree->SetBranchAddress("B_s0_PZ_kaon0",&h_LOKI_PZ[1]);
  intree->SetBranchAddress("B_s0_PZ_kaon3",&h_LOKI_PZ[2]);
  intree->SetBranchAddress("B_s0_PZ_kaon2",&h_LOKI_PZ[3]);
/*BCON PX Branches*************************************************************/
  double h_BCON_PX[4];
  intree->SetBranchAddress("B_s0_BCON_PX_kaon1",&h_BCON_PX[0]);
  intree->SetBranchAddress("B_s0_BCON_PX_kaon0",&h_BCON_PX[1]);
  intree->SetBranchAddress("B_s0_BCON_PX_kaon3",&h_BCON_PX[2]);
  intree->SetBranchAddress("B_s0_BCON_PX_kaon2",&h_BCON_PX[3]);
/*BCON PY Branches*************************************************************/
  double h_BCON_PY[4];
  intree->SetBranchAddress("B_s0_BCON_PY_kaon1",&h_BCON_PY[0]);
  intree->SetBranchAddress("B_s0_BCON_PY_kaon0",&h_BCON_PY[1]);
  intree->SetBranchAddress("B_s0_BCON_PY_kaon3",&h_BCON_PY[2]);
  intree->SetBranchAddress("B_s0_BCON_PY_kaon2",&h_BCON_PY[3]);
/*BCON PZ Branches*************************************************************/
  double h_BCON_PZ[4];
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon1",&h_BCON_PZ[0]);
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon0",&h_BCON_PZ[1]);
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon3",&h_BCON_PZ[2]);
  intree->SetBranchAddress("B_s0_BCON_PZ_kaon2",&h_BCON_PZ[3]);
/*MC only branches*************************************************************/
  double h_TRUEPX[4];
  double h_TRUEPY[4];
  double h_TRUEPZ[4];
  if(isMC)
  {
/*TRUEPX Branches**************************************************************/
    intree->SetBranchAddress("Kminus_TRUEP_X", &h_TRUEPX[0]);
    intree->SetBranchAddress("Kplus_TRUEP_X",  &h_TRUEPX[1]);
    intree->SetBranchAddress("Kminus0_TRUEP_X",&h_TRUEPX[2]);
    intree->SetBranchAddress("Kplus0_TRUEP_X", &h_TRUEPX[3]);
/*TRUEPY Branches**************************************************************/
    intree->SetBranchAddress("Kminus_TRUEP_Y", &h_TRUEPY[0]);
    intree->SetBranchAddress("Kplus_TRUEP_Y",  &h_TRUEPY[1]);
    intree->SetBranchAddress("Kminus0_TRUEP_Y",&h_TRUEPY[2]);
    intree->SetBranchAddress("Kplus0_TRUEP_Y", &h_TRUEPY[3]);
/*TRUEPZ Branches**************************************************************/
    intree->SetBranchAddress("Kminus_TRUEP_Z", &h_TRUEPZ[0]);
    intree->SetBranchAddress("Kplus_TRUEP_Z",  &h_TRUEPZ[1]);
    intree->SetBranchAddress("Kminus0_TRUEP_Z",&h_TRUEPZ[2]);
    intree->SetBranchAddress("Kplus0_TRUEP_Z", &h_TRUEPZ[3]);
  }
/*ProbNNp Branches*************************************************************/
  double h_ProbNNp[4];
  intree->SetBranchAddress("Kminus_ProbNNp", &h_ProbNNp[0]);
  intree->SetBranchAddress("Kplus_ProbNNp",  &h_ProbNNp[1]);
  intree->SetBranchAddress("Kminus0_ProbNNp",&h_ProbNNp[2]);
  intree->SetBranchAddress("Kplus0_ProbNNp", &h_ProbNNp[3]);
/*ProbNNk Branches*************************************************************/
  double h_ProbNNk[4];
  intree->SetBranchAddress("Kminus_ProbNNk", &h_ProbNNk[0]);
  intree->SetBranchAddress("Kplus_ProbNNk",  &h_ProbNNk[1]);
  intree->SetBranchAddress("Kminus0_ProbNNk",&h_ProbNNk[2]);
  intree->SetBranchAddress("Kplus0_ProbNNk", &h_ProbNNk[3]);
/*ProbNNpi Branches************************************************************/
  double h_ProbNNpi[4];
  intree->SetBranchAddress("Kminus_ProbNNpi", &h_ProbNNpi[0]);
  intree->SetBranchAddress("Kplus_ProbNNpi",  &h_ProbNNpi[1]);
  intree->SetBranchAddress("Kminus0_ProbNNpi",&h_ProbNNpi[2]);
  intree->SetBranchAddress("Kplus0_ProbNNpi", &h_ProbNNpi[3]);
/*IPCHI2_OWNPV Branches********************************************************/
  double h_IPCHI2_OWNPV[4];
  intree->SetBranchAddress("Kminus_IPCHI2_OWNPV", &h_IPCHI2_OWNPV[0]);
  intree->SetBranchAddress("Kplus_IPCHI2_OWNPV",  &h_IPCHI2_OWNPV[1]);
  intree->SetBranchAddress("Kminus0_IPCHI2_OWNPV",&h_IPCHI2_OWNPV[2]);
  intree->SetBranchAddress("Kplus0_IPCHI2_OWNPV", &h_IPCHI2_OWNPV[3]);
/*BDT branches*****************************************************************/
  float B_s0_ln_FDCHI2; outtree->Branch("B_s0_ln_FDCHI2", &B_s0_ln_FDCHI2, "B_s0_ln_FDCHI2/F" );
  float B_s0_ln_IPCHI2; outtree->Branch("B_s0_ln_IPCHI2", &B_s0_ln_IPCHI2, "B_s0_ln_IPCHI2/F" );
  float B_s0_ln_EVCHI2; outtree->Branch("B_s0_ln_EVCHI2", &B_s0_ln_EVCHI2, "B_s0_ln_EVCHI2/F" );
  float B_s0_PT_fiveGeV;outtree->Branch("B_s0_PT_fiveGeV",&B_s0_PT_fiveGeV,"B_s0_PT_fiveGeV/F");
  float B_s0_Eta;       outtree->Branch("B_s0_Eta",       &B_s0_Eta,       "B_s0_Eta/F"       );
  float minK_ln_IPCHI2; outtree->Branch("minK_ln_IPCHI2", &minK_ln_IPCHI2, "minK_ln_IPCHI2/F" );
  float minK_PT_GeV;    outtree->Branch("minK_PT_GeV",    &minK_PT_GeV,    "minK_PT_GeV/F"    );
  double h_LOKI_PT[4]; // Necessary for min Kaon PT
/*New mass branches************************************************************/
  // phi KK
  TLorentzVector BCON_KK_P;   double BCON_KK_M;         outtree->Branch("BCON_KK_M",  &BCON_KK_M,  "BCON_KK_M/D"  );
  TLorentzVector KK_TRUEP;    double KK_TRUEM; if(isMC) outtree->Branch("KK_TRUEM",   &KK_TRUEM,   "KK_TRUEM/D"   );
  TLorentzVector phiKplusP;   double phiKplusM;         outtree->Branch("phiKplusM",  &phiKplusM,  "phiKplusM/D"  );
  TLorentzVector phiKminusP;  double phiKminusM;        outtree->Branch("phiKminusM", &phiKminusM, "phiKminusM/D" );
  // phi pipi
  TLorentzVector phipipiP;    double phipipiM;          outtree->Branch("phipipiM",   &phipipiM,   "phipipiM/D"   );
  TLorentzVector pipiP;       double pipiM;             outtree->Branch("pipiM",      &pipiM,      "pipiM/D"      );
  TLorentzVector phipiplusP;  double phipiplusM;        outtree->Branch("phipiplusM", &phipiplusM, "phipiplusM/D" );
  TLorentzVector phipiminusP; double phipiminusM;       outtree->Branch("phipiminusM",&phipiminusM,"phipiminusM/D");
  // phi K pi
  TLorentzVector KpiP;        double KpiM;              outtree->Branch("KpiM",       &KpiM,       "KpiM/D"       );
  TLorentzVector phiKpiP;     double phiKpiM;           outtree->Branch("phiKpiM",    &phiKpiM,    "phiKpiM/D"    );
  // phi K p
  TLorentzVector phipP;       double phipM;             outtree->Branch("phipM",      &phipM,      "phipM/D"      );
  TLorentzVector phipbarP;    double phipbarM;          outtree->Branch("phipbarM",   &phipbarM,   "phipbarM/D"   );
  TLorentzVector KpP;         double KpM;               outtree->Branch("KpM",        &KpM,        "KpM/D"        );
  TLorentzVector phiKpP;      double phiKpM;            outtree->Branch("phiKpM",     &phiKpM,     "phiKpM/D"     );
  // phi pi p
  TLorentzVector pipP;        double pipM;              outtree->Branch("pipM",       &pipM,       "pipM/D"       );
  TLorentzVector pipbarP;     double pipbarM;           outtree->Branch("pipbarM",    &pipbarM,    "pibarM/D"     );
  TLorentzVector phipipP;     double phipipM;           outtree->Branch("phipipM",    &phipipM,    "phipipM/D"    );
  TLorentzVector phipipbarP;  double phipipbarM;        outtree->Branch("phipipbarM", &phipipbarM, "phipipbarM/D" );
  // PID variables for tracks identified as something else
  double pion_ProbNNk;    outtree->Branch("pion_ProbNNk",   &pion_ProbNNk,   "pion_ProbNNk/D" );
  double pion_ProbNNpi;   outtree->Branch("pion_ProbNNpi",  &pion_ProbNNpi,  "pion_ProbNNpi/D");
  double pion_ProbNNp;    outtree->Branch("pion_ProbNNp",   &pion_ProbNNp,   "pion_ProbNNp/D" );
  double proton_ProbNNk;  outtree->Branch("proton_ProbNNk", &proton_ProbNNk, "proton_ProbNNk/D" );
  double proton_ProbNNpi; outtree->Branch("proton_ProbNNpi",&proton_ProbNNpi,"proton_ProbNNpi/D");
  double proton_ProbNNp;  outtree->Branch("proton_ProbNNp", &proton_ProbNNp, "proton_ProbNNp/D" );
/*Helicity angle branches******************************************************/
  // Track 4-momentum in B frame and daughter frames
  TLorentzVector Bframe_h_P[4], /*Bframe_d_P[4],*/ dframe_h_P[4], dframe_other_h_P[4];
  TVector3 Bframe_e, dframe_e[2], dframe_n[2];
//  TVector3 CrossProduct[2];
  double Phi_angle; outtree->Branch("Phi_angle", &Phi_angle, "Phi_angle/D" );
  double cos_Phi  ; outtree->Branch("cos_Phi"  , &cos_Phi  , "cos_Phi/D"   );
  double sin_Phi  ; outtree->Branch("sin_Phi"  , &sin_Phi  , "sin_Phi/D"   );
  double cos_theta[2];
  outtree->Branch("cos_theta1",&cos_theta[0],"cos_theta1/D");
  outtree->Branch("cos_theta2",&cos_theta[1],"cos_theta2/D");
  // Same branches, but with constrained B mass.
  double BCON_Phi_angle; outtree->Branch("BCON_Phi_angle", &BCON_Phi_angle, "BCON_Phi_angle/D" );
  double BCON_cos_Phi  ; outtree->Branch("BCON_cos_Phi"  , &BCON_cos_Phi  , "BCON_cos_Phi/D"   );
  double BCON_sin_Phi  ; outtree->Branch("BCON_sin_Phi"  , &BCON_sin_Phi  , "BCON_sin_Phi/D"   );
  double BCON_cos_theta[2];
  outtree->Branch("BCON_cos_theta1",&BCON_cos_theta[0],"BCON_cos_theta1/D");
  outtree->Branch("BCON_cos_theta2",&BCON_cos_theta[1],"BCON_cos_theta2/D");
/*Event loop*******************************************************************/
  for(int i = 0; i < n; i++)
  {
    intree->GetEntry(i);
/*BDT variables****************************************************************/
    B_s0_ln_FDCHI2  = safeLog(B_s0_FDCHI2_OWNPV);
    B_s0_ln_IPCHI2  = safeLog(B_s0_IPCHI2_OWNPV);
    B_s0_ln_EVCHI2  = safeLog(B_s0_ENDVERTEX_CHI2/5.0); // 5 degrees of freedom
    B_s0_PT_fiveGeV = B_s0_PT/5000; // Change units for sensible range of numbers
    B_s0_Eta        = TMath::ACosH(B_s0_P/B_s0_PT);
    minK_ln_IPCHI2  = safeLog(*min_element(h_IPCHI2_OWNPV,h_IPCHI2_OWNPV+4));
    for(int j = 0; j < 4; j++) h_LOKI_PT[j] = TMath::Sqrt(TMath::Power(h_LOKI_PX[j],2)+TMath::Power(h_LOKI_PY[j],2));
    minK_PT_GeV     = (*min_element(h_LOKI_PT,h_LOKI_PT+4))/1000;
/*Mass branches****************************************************************/
    // MC truth mass
    if(isMC)
    {
      for(int j = 0; j < 4; j++) h_TRUEP[j].SetXYZM(h_TRUEPX[j],h_TRUEPY[j],h_TRUEPZ[j],Kmass);
      KK_TRUEP = h_TRUEP[2] + h_TRUEP[3];
      KK_TRUEM = KK_TRUEP.M();
    }
    // Track 4-momenta with constrained Bs mass
    for(int j = 0; j < 4; j++) h_BCONP[j].SetXYZM(h_BCON_PX[j],h_BCON_PY[j],h_BCON_PZ[j],Kmass);
    BCON_KK_P = h_BCONP[2] + h_BCONP[3];
    BCON_KK_M = BCON_KK_P.M();
    // Initial 4K hypothesis
    for(int j = 0; j < 4; j++) hP[j].SetXYZM(h_LOKI_PX[j],h_LOKI_PY[j],h_LOKI_PZ[j],Kmass);
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
    // PID branches
    pion_ProbNNk  = h_ProbNNk[pion] ;
    pion_ProbNNpi = h_ProbNNpi[pion];
    pion_ProbNNp  = h_ProbNNp[pion] ;
    // phi K pi
    pionP.SetXYZM(h_LOKI_PX[pion],h_LOKI_PY[pion],h_LOKI_PZ[pion],pimass);
    phiKpiP = hP[0] + hP[1] + pionP + hP[kaon];
    phiKpiM = phiKpiP.M();
    // K pi
    KpiP = pionP + hP[kaon];
    KpiM = KpiP.M();
    // phi pi-
    pionP.SetXYZM(h_LOKI_PX[2],h_LOKI_PY[2],h_LOKI_PZ[2],pimass);
    pipiP       = pionP; // Half of the pi pi 4momentum
    phipiminusP = hP[0] + hP[1] + pionP;
    phipiminusM = phipiminusP.M();
    phipipiP    = phipiminusP; // 3/4 of phi pi pi 4momentum
    // phi pi+
    pionP.SetXYZM(h_LOKI_PX[3],h_LOKI_PY[3],h_LOKI_PZ[3],pimass);
    pipiP     += pionP; // Other half of the pi pi 4momentum
    phipipiP  += pionP; // Other 1/4 of the phi pi pi 4momentum
    phipiplusP = hP[0] + hP[1] + pionP;
    phipiplusM = phipiplusP.M();
    // phi pi pi and pi pi
    phipipiM   = phipipiP.M();
    pipiM      = pipiP.M();
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
    // PID branches
    proton_ProbNNk  = h_ProbNNk[proton] ;
    proton_ProbNNpi = h_ProbNNpi[proton];
    proton_ProbNNp  = h_ProbNNp[proton] ;
    // phi K proton
    protonP.SetXYZM(h_LOKI_PX[proton],h_LOKI_PY[proton],h_LOKI_PZ[proton],pmass);
    phiKpP = hP[0] + hP[1] + protonP + hP[kaon];
    phiKpM = phiKpP.M();
    // K proton
    KpP = protonP + hP[kaon];
    KpM = KpP.M();
    // phi proton
    protonP.SetXYZM(h_LOKI_PX[3],h_LOKI_PY[3],h_LOKI_PZ[3],pmass);
    phipP = hP[0] + hP[1] + protonP;
    phipM = phipP.M();
    // phi anti-proton
    protonP.SetXYZM(h_LOKI_PX[2],h_LOKI_PY[2],h_LOKI_PZ[2],pmass);
    phipbarP = hP[0] + hP[1] + protonP;
    phipbarM = phipbarP.M();
    // phi pi− proton
    protonP.SetXYZM(h_LOKI_PX[3],h_LOKI_PY[3],h_LOKI_PZ[3],pmass);
    pionP.SetXYZM(h_LOKI_PX[2],h_LOKI_PY[2],h_LOKI_PZ[2],pimass);
    pipP = pionP + protonP;
    pipM = pipP.M();
    phipipP = hP[0] + hP[1] + protonP + pionP;
    phipipM = phipipP.M();
    // phi pi+ anti-proton
    protonP.SetXYZM(h_LOKI_PX[2],h_LOKI_PY[2],h_LOKI_PZ[2],pmass);
    pionP.SetXYZM(h_LOKI_PX[3],h_LOKI_PY[3],h_LOKI_PZ[3],pimass);
    pipbarP = pionP + protonP;
    pipbarM = pipP.M();
    phipipbarP = hP[0] + hP[1] + protonP + pionP;
    phipipbarM = phipipbarP.M();
/*Helicity angles**************************************************************/
/*******************************************************************************
    See page 12 of LHCb-ANA-2012-067. Replace muons with the resonant kaons.
*******************************************************************************/
    for(int BCON = 0; BCON < 2; BCON++)
    {
      if(BCON==1)
      {
        BP    = h_BCONP[0] + h_BCONP[1] + h_BCONP[2] + h_BCONP[3];
        dP[0] = h_BCONP[0] + h_BCONP[1];
        dP[1] = h_BCONP[2] + h_BCONP[3];
      }
      // Loop over Kaons
      for(int j = 0; j < 4; j++)
      {
        // Boost into B frame
        Bframe_h_P[j] = BCON==1 ? h_BCONP[j] : hP[j];
        Bframe_h_P[j].Boost(-BP.BoostVector());
        // Boost into daughter frames
        dframe_h_P[j] = BCON==1 ? h_BCONP[j] : hP[j];
        dframe_h_P[j].Boost(-dP[j/2].BoostVector());
        // Boost into "wrong" daughter frames
        // Thanks Dianne!
        dframe_other_h_P[j] = BCON==1 ? h_BCONP[j] : hP[j];
        dframe_other_h_P[j].Boost(-dP[(j-3)/-2].BoostVector());
      }
      // Loop over daughters
      for(int j = 0; j < 2; j++)
      {
        int minus = 2*j;   // 0 and 2
        int plus  = 2*j+1; // 1 and 3
        dframe_e[j] = -1.0 * ((dframe_other_h_P[minus].Vect() + dframe_other_h_P[plus].Vect()) * (1.0/(dframe_other_h_P[minus].Vect() + dframe_other_h_P[plus].Vect()).Mag()));
        if(BCON == 0)
        {
          cos_theta[j] = (dframe_h_P[plus].Vect() * (1.0/dframe_h_P[plus].Vect().Mag())).Dot(dframe_e[j]);
        }
        else
        {
          BCON_cos_theta[j] = (dframe_h_P[plus].Vect() * (1.0/dframe_h_P[plus].Vect().Mag())).Dot(dframe_e[j]);  
        }    
        dframe_n[j] = (Bframe_h_P[plus].Vect().Cross(Bframe_h_P[minus].Vect())) * (1.0/(Bframe_h_P[plus].Vect().Cross(Bframe_h_P[minus].Vect())).Mag());
      }
      Bframe_e = (Bframe_h_P[0].Vect() + Bframe_h_P[1].Vect()) * (1.0/(Bframe_h_P[0].Vect() + Bframe_h_P[1].Vect()).Mag());
      if(BCON == 0)
      {
        cos_Phi = dframe_n[1].Dot(dframe_n[0]);
        sin_Phi = (dframe_n[1].Cross(dframe_n[0])).Dot(Bframe_e);
        Phi_angle  = TMath::ACos(cos_Phi) * (sin_Phi/TMath::Abs(sin_Phi));
      }
      else
      {
        BCON_cos_Phi = dframe_n[1].Dot(dframe_n[0]);
        BCON_sin_Phi = (dframe_n[1].Cross(dframe_n[0])).Dot(Bframe_e);
        BCON_Phi_angle  = TMath::ACos(BCON_cos_Phi) * (BCON_sin_Phi/TMath::Abs(BCON_sin_Phi));
      }
    }
    outtree->Fill();
  }
/*Write the output*************************************************************/
  cout << "Writing to " << outputfilename << endl;
  outtree->Write();
  infile->Close();
  outfile->Close();
  return;
}
int main(int argc, char* argv[])
{
  if(argc == 3)
  {
    addBranches((string)argv[1],(string)argv[2]);
    return 0;
  }
  else
  {
    cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
    return 1;
  }
}
