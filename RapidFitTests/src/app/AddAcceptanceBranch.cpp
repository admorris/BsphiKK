// Std headers
#include <string.h>
#include <iostream>
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// RapidFit headers
#include "LegendreMomentShape.h"
// Custom headers
#include "progbar.h"
#include "GetTree.h"
using namespace std;
void addBranches(string ntuplefile = "BsphiKK_data", string accfile = "LegendreMoments_Acceptance.root")
{
  cout << "Adding branches to " << ntuplefile << endl;
/*Input************************************************************************/
  size_t ext_pos = ntuplefile.find(".root");
  // Open the input file and create the output file
  TFile* infile  = new TFile(ntuplefile.c_str()),
       * outfile = new TFile((ntuplefile.substr(0,ext_pos)+"_acc_weights.root").c_str(),"RECREATE");
  // Get the input tree and create an empty output tree
  TTree* intree  = GetTree(infile);
  outfile->cd();
  TTree* outtree = intree->CloneTree(0);
  // Read the number of events in the input file
  Int_t n = intree->GetEntries();
/*Input branches***************************************************************/
  double mKK     ; intree->SetBranchAddress("KK_M"      , &mKK     );
  double phi     ; intree->SetBranchAddress("Phi_angle" , &phi     );
  double ctheta_1; intree->SetBranchAddress("cos_theta1", &ctheta_1);
  double ctheta_2; intree->SetBranchAddress("cos_theta2", &ctheta_2);
/*New branches*****************************************************************/
  double acc_w; outtree->Branch("acc_w", &acc_w, "acc_w/D");
/*Initialise the acceptance object*********************************************/
  LegendreMomentShape acc(accfile);
/*Event loop*******************************************************************/
  progbar bar(n);
  for(Int_t i = 0; i < n; i++)
  {
    intree->GetEntry(i);
/*Calculate acceptance*********************************************************/
    acc_w = acc.Evaluate(mKK, phi, ctheta_1, ctheta_2);
/*Fill tree and show progress**************************************************/
    outtree->Fill();
    if(i%(n/100) == 0)
    {
      bar.print(i);
    }
  }
  bar.terminate();
/*Write the output*************************************************************/
  outfile->cd();
  outtree->Write();
  infile->Close();
  outfile->Close();
  return;
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    cout << "Usage: " << argv[0] << " <ntuple file> [<legendre moments file>]" << endl;
    return 1;
  }
  else if(argc==2)
    addBranches((string)argv[1]);
  else if(argc==3)
    addBranches((string)argv[1],(string)argv[2]);
  else if(argc>3)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  
  return 0;
}
