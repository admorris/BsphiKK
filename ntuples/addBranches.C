#include <string.h>
#include <time.h>
#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TStyle.h"
#include "./progbar.h"
using namespace std;
Double_t safeLog(Double_t x)
{
  if(x>TMath::Exp(-25)) return TMath::Log(x);
  else return -25;
}
void addBranches(string filename)
{
  const Int_t t0 = time(0);
  cout << filename << endl;
/*Input************************************************************************/
  TFile* infile = new TFile((filename+"_cuts.root").c_str()),
    * outfile = new TFile((filename+"_bdtVars.root").c_str(),"RECREATE");
  TTree* intree = (TTree*)infile->Get("DecayTree"),
    * outtree = intree->CloneTree(0);
  Int_t n = intree->GetEntries();
/*PDG masses*******************************************************************/
  Double_t
    Kmass = 493.667, pimass = 139.570, pmass = 938.272;
/*Variables for new branches***************************************************/
  Int_t pion,proton,kaon;
  TLorentzVector hP[4],pionP,protonP;
/*Input branches***************************************************************/
  Double_t B_s0_FDCHI2_OWNPV; intree->SetBranchAddress("B_s0_FDCHI2_OWNPV",&B_s0_FDCHI2_OWNPV);
  Double_t B_s0_IPCHI2_OWNPV; intree->SetBranchAddress("B_s0_IPCHI2_OWNPV",&B_s0_IPCHI2_OWNPV);
  Double_t B_s0_ENDVERTEX_CHI2; intree->SetBranchAddress("B_s0_ENDVERTEX_CHI2",&B_s0_ENDVERTEX_CHI2);
  Double_t B_s0_P; intree->SetBranchAddress("B_s0_P",&B_s0_P);
  Double_t B_s0_PT; intree->SetBranchAddress("B_s0_PT",&B_s0_PT);
/*PX Branches******************************************************************/
  Double_t h_PX[4];
  intree->SetBranchAddress("Kminus_PX",&h_PX[0]);
  intree->SetBranchAddress("Kplus_PX",&h_PX[1]);
  intree->SetBranchAddress("Kminus0_PX",&h_PX[2]);
  intree->SetBranchAddress("Kplus0_PX",&h_PX[3]);
/*PY Branches******************************************************************/
  Double_t h_PY[4];
  intree->SetBranchAddress("Kminus_PY",&h_PY[0]);
  intree->SetBranchAddress("Kplus_PY",&h_PY[1]);
  intree->SetBranchAddress("Kminus0_PY",&h_PY[2]);
  intree->SetBranchAddress("Kplus0_PY",&h_PY[3]);
/*PZ Branches******************************************************************/
  Double_t h_PZ[4];
  intree->SetBranchAddress("Kminus_PZ",&h_PZ[0]);
  intree->SetBranchAddress("Kplus_PZ",&h_PZ[1]);
  intree->SetBranchAddress("Kminus0_PZ",&h_PZ[2]);
  intree->SetBranchAddress("Kplus0_PZ",&h_PZ[3]);
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
  intree->SetBranchAddress("Kminus_ProbNNp",&h_ProbNNp[0]);
  intree->SetBranchAddress("Kplus_ProbNNp",&h_ProbNNp[1]);
  intree->SetBranchAddress("Kminus0_ProbNNp",&h_ProbNNp[2]);
  intree->SetBranchAddress("Kplus0_ProbNNp",&h_ProbNNp[3]);
/*ProbNNk Branches*************************************************************/
  Double_t h_ProbNNk[4];
  intree->SetBranchAddress("Kminus_ProbNNk",&h_ProbNNk[0]);
  intree->SetBranchAddress("Kplus_ProbNNk",&h_ProbNNk[1]);
  intree->SetBranchAddress("Kminus0_ProbNNk",&h_ProbNNk[2]);
  intree->SetBranchAddress("Kplus0_ProbNNk",&h_ProbNNk[3]);
/*ProbNNpi Branches************************************************************/
  Double_t h_ProbNNpi[4];
  intree->SetBranchAddress("Kminus_ProbNNpi",&h_ProbNNpi[0]);
  intree->SetBranchAddress("Kplus_ProbNNpi",&h_ProbNNpi[1]);
  intree->SetBranchAddress("Kminus0_ProbNNpi",&h_ProbNNpi[2]);
  intree->SetBranchAddress("Kplus0_ProbNNpi",&h_ProbNNpi[3]);
/*BDT branches*****************************************************************/
  Double_t B_s0_ln_FDCHI2; outtree->Branch("B_s0_ln_FDCHI2",&B_s0_ln_FDCHI2,"B_s0_ln_FDCHI2/D");
  Double_t B_s0_ln_IPCHI2; outtree->Branch("B_s0_ln_IPCHI2",&B_s0_ln_IPCHI2,"B_s0_ln_IPCHI2/D");
  Double_t B_s0_ln_EVCHI2; outtree->Branch("B_s0_ln_EVCHI2",&B_s0_ln_EVCHI2,"B_s0_ln_EVCHI2/D");
  Double_t B_s0_PT_GeV; outtree->Branch("B_s0_PT_GeV",&B_s0_PT_GeV,"B_s0_PT_GeV/D");
  Double_t B_s0_Eta; outtree->Branch("B_s0_Eta",&B_s0_Eta,"B_s0_Eta/D");
/*New mass branches************************************************************/
  TLorentzVector BCON_KK_P; Double_t BCON_KK_M; outtree->Branch("BCON_KK_M",&BCON_KK_M,"BCON_KK_M/D");
  TLorentzVector phiKplusP; Double_t phiKplusM; outtree->Branch("phiKplusM",&phiKplusM,"phiKplusM/D");
  TLorentzVector phiKminusP; Double_t phiKminusM; outtree->Branch("phiKminusM",&phiKminusM,"phiKminusM/D");
  TLorentzVector phipiplusP; Double_t phipiplusM; outtree->Branch("phipiplusM",&phipiplusM,"phipiplusM/D");
  TLorentzVector phipiminusP; Double_t phipiminusM; outtree->Branch("phipiminusM",&phipiminusM,"phipiminusM/D");
  TLorentzVector phiKpiP; Double_t phiKpiM; outtree->Branch("phiKpiM",&phiKpiM,"phiKpiM/D");
  TLorentzVector phipP; Double_t phipM; outtree->Branch("phipM",&phipM,"phipM/D");
  TLorentzVector phipbarP; Double_t phipbarM; outtree->Branch("phipbarM",&phipbarM,"phipbarM/D");
  TLorentzVector phiKpP; Double_t phiKpM; outtree->Branch("phiKpM",&phiKpM,"phiKpM/D");
/*Event loop*******************************************************************/
  for(Int_t i = 0; i < n; i++)
  {
    intree->GetEntry(i);
/*BDT variables****************************************************************/
    B_s0_ln_FDCHI2 = safeLog(B_s0_FDCHI2_OWNPV);
    B_s0_ln_IPCHI2 = safeLog(B_s0_IPCHI2_OWNPV);
    B_s0_ln_EVCHI2 = safeLog(B_s0_ENDVERTEX_CHI2/5.0); // 5 degrees of freedom
    B_s0_PT_GeV = B_s0_PT/1000;
    B_s0_Eta = TMath::ACosH(B_s0_P/B_s0_PT);
/*Mass branches****************************************************************/
    for(Int_t j = 0; j < 4; j++) hP[j].SetXYZM(h_BCON_PX[j],h_BCON_PY[j],h_BCON_PZ[j],Kmass);
    BCON_KK_P = hP[2]+hP[3];
    BCON_KK_M = BCON_KK_P.M();
    // Initial 4K hypothesis
    for(Int_t j = 0; j < 4; j++) hP[j].SetXYZM(h_PX[j],h_PY[j],h_PZ[j],Kmass);
    // Both phiK+− branches
    phiKminusP = hP[0]+hP[1]+hP[2];
    phiKminusM = phiKminusP.M();
    phiKplusP = hP[0]+hP[1]+hP[3];
    phiKplusM = phiKplusP.M();
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
    // Set branch value
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
    // Decide which is the proton for phi K p
    if(h_ProbNNp[2] > h_ProbNNp[3])
    {
      // first is the proton
      proton = 2;
      kaon = 3;
    }
    else
    {
      // second is the proton
      proton = 3;
      kaon = 2;
    }
    // Set branch value
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
/*Fill tree and show progress**************************************************/
    outtree->Fill();
    if(i%100==0)
    {
      progbar(i,n);
      timestamp(t0);
    }
  }
  cout << endl;
/*Output***********************************************************************/
  outtree->Write();
  infile->Close();
  outfile->Close();
  return;
}



