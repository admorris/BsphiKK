#include <string.h>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "GetTree.h"
using namespace std;
int count(string str, string substr)
{
  int retval(0);
  if(str.size()<substr.size()) return 0; // If the substring doesn't fit in the full then it won't appear
  unsigned i(0);
  if(str.rfind("/") != string::npos) i = str.rfind("/");
  for(; i+substr.size() <= str.size(); i++)
  {
    if(str.substr(i,substr.size()).compare(substr)==0) retval++;
  }
  return retval;
}
void addBranches(string filename = "Bsphiphi")
{
  double m_K = 493.677;
  int nphi = count(filename, "phi");
  int nBs = count(filename, "Bs");
  cout << "Adding branches to " << filename << endl;
/*Input************************************************************************/
  // Open the input file and create the output file
  TFile* infile  = TFile::Open((filename+".root"   ).c_str()),
       * outfile = TFile::Open((filename+"_branches.root").c_str(),"RECREATE");
  // Get the input tree and create an empty output tree
  TTree* intree  = GetTree(infile),
       * outtree = intree->CloneTree(0);
  // Read the number of events in the input file
  Int_t n = intree->GetEntries();
  TLorentzVector Kp_0_P, Km_0_P, Kp_1_P, Km_1_P, phi_P, KK_P;
  double m_b, m_phi, m_kk;
  double M_B, M_PHI, M_KK, M_phiKp, M_phiKm;
  double Kp_0_PX, Km_0_PX, Kp_1_PX, Km_1_PX;
  double Kp_0_PY, Km_0_PY, Kp_1_PY, Km_1_PY;
  double Kp_0_PZ, Km_0_PZ, Kp_1_PZ, Km_1_PZ;
  intree->SetBranchAddress("Kp_0_PX",&Kp_0_PX);
  intree->SetBranchAddress("Kp_0_PY",&Kp_0_PY);
  intree->SetBranchAddress("Kp_0_PZ",&Kp_0_PZ);
  intree->SetBranchAddress("Km_0_PX",&Km_0_PX);
  intree->SetBranchAddress("Km_0_PY",&Km_0_PY);
  intree->SetBranchAddress("Km_0_PZ",&Km_0_PZ);
  if(nBs == 1)
  {
    intree->SetBranchAddress("Bs0_0_M",&m_b);
    intree->SetBranchAddress("Kp_1_PX",&Kp_1_PX);
    intree->SetBranchAddress("Kp_1_PY",&Kp_1_PY);
    intree->SetBranchAddress("Kp_1_PZ",&Kp_1_PZ);
    intree->SetBranchAddress("Km_1_PX",&Km_1_PX);
    intree->SetBranchAddress("Km_1_PY",&Km_1_PY);
    intree->SetBranchAddress("Km_1_PZ",&Km_1_PZ);
    outtree->Branch("B_s0_M",&M_B);
    outtree->Branch("KK_M",&M_KK);
    outtree->Branch("phiKplusM",&M_phiKm);
    outtree->Branch("phiKminusM",&M_phiKp);
    cout << "Creating branch B_s0_M" << endl;
    cout << "Creating branch KK_M" << endl;
    cout << "Creating branch phiKplusM" << endl;
    cout << "Creating branch phiKminusM" << endl;
  }
  outtree->Branch("phi_1020_M",&M_PHI);
  cout << "Creating branch phi_1020_M" << endl;
  if(nphi>0)
  {
    intree->SetBranchAddress("phi_0_M",&m_phi);
    cout << "Can just read the branch phi_0_M" << endl;
  }
  if(nphi==2)
  {
    intree->SetBranchAddress("phi_1_M",&m_kk);
    cout << "Can just read the branch phi_1_M" << endl;
  }
/*Event loop*******************************************************************/
  for(Int_t i = 0; i < n; i++)
  {
    intree->GetEntry(i);
    if(nBs == 1)
    {
      M_B = m_b*1e3;
      Kp_0_P.SetXYZM(Kp_0_PX,Kp_0_PY,Kp_0_PZ,m_K);
      Km_0_P.SetXYZM(Km_0_PX,Km_0_PY,Km_0_PZ,m_K);
      Kp_1_P.SetXYZM(Kp_1_PX,Kp_1_PY,Kp_1_PZ,m_K);
      Km_1_P.SetXYZM(Km_1_PX,Km_1_PY,Km_1_PZ,m_K);
      switch(nphi)
      {
        case 0:
          M_PHI = (Kp_0_P+Km_0_P).M()*1e3;
          M_KK = (Kp_1_P+Km_1_P).M()*1e3;
          break;
        case 1:
          M_PHI = m_phi*1e3;
          M_KK = (Kp_1_P+Km_1_P).M()*1e3;
          break;
        case 2:
          M_PHI = m_phi*1e3;
          M_KK = m_kk*1e3;
      }
      M_phiKp = (Kp_0_P+Km_0_P+Kp_1_P).M()*1e3;
      M_phiKm = (Kp_0_P+Km_0_P+Km_1_P).M()*1e3;
    }
    else
    {
      M_PHI = m_phi*1e3;
    }
/*Fill tree********************************************************************/
    outtree->Fill();
    if(i%100000 == 0)
      cout << "Event " << i << "/" << n << endl;
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
  addBranches(((string)argv[1]).substr(0,((string)argv[1]).find(".root")));
  return 0;
}
