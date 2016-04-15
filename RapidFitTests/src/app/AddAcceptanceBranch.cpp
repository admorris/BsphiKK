// Std headers
#include <string.h>
#include <iostream>
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// RapidFit headers
#include "LegendreMomentShape.h"
// GSL headers
#include <gsl/gsl_sf_legendre.h>
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
  Int_t nevt = intree->GetEntries();
/*Set up variables for Greig's method******************************************/
  static const int l_max = 2;
  static const int i_max = 2;
  static const int k_max = 2;
  static const int j_max = 2;
  double c[l_max+1][i_max+1][k_max+1][j_max+1];
  for ( int l = 0; l < l_max + 1; l++ )
    for ( int i = 0; i < i_max + 1; i++ )
      for ( int k = 0; k < k_max + 1; k++ )
        for ( int j = 0; j < j_max + 1; j++ )
          c[l][i][k][j] = 0.;
  // Hard-coded coefficients
  c[0][0][0][0] = 0.065958;
  c[0][1][1][2] = 0.052493;
  c[0][2][0][0] = -0.007075;
  c[0][2][2][2] = -0.022391;
  c[1][0][0][0] = -0.012124;
  c[1][0][0][2] = -0.004632;
  // Angular functions
  double Q_l(0.);
  double P_i(0.);
  double Y_jk(0.);
  double mKK_mapped;
/******************************************************************************/
/*Input branches***************************************************************/
  double mKK     ; intree->SetBranchAddress("KK_M"      , &mKK     );
  double phi     ; intree->SetBranchAddress("Phi_angle" , &phi     );
  double ctheta_1; intree->SetBranchAddress("cos_theta1", &ctheta_1);
  double ctheta_2; intree->SetBranchAddress("cos_theta2", &ctheta_2);
/*New branches*****************************************************************/
  double acc_w; outtree->Branch("acc_w", &acc_w, "acc_w/D");
  double acc_g; outtree->Branch("acc_g", &acc_g, "acc_g/D");
/*Initialise the acceptance object*********************************************/
  LegendreMomentShape acc(accfile);
  double mKK_min = acc.mKK_min;
  double mKK_max = acc.mKK_max;
/*Event loop*******************************************************************/
  progbar bar(nevt);
  for(Int_t ievt = 0; ievt < nevt; ievt++)
  {
    intree->GetEntry(ievt);
/*Calculate acceptance*********************************************************/
    acc_w = acc.Evaluate(mKK, phi, ctheta_1, ctheta_2);
    acc_g = 0;
    mKK_mapped = 2.*(mKK-mKK_min)/(mKK_max-mKK_min) -1;
    if(abs(mKK_mapped)<1)
      for ( int l = 0; l < l_max + 1; l++ )
        for ( int i = 0; i < i_max + 1; i++ )
          for ( int k = 0; k < k_max + 1; k++ )
            for ( int j = 0; j < j_max + 1; j++ )
            {
              if (j < k) continue; // must have l >= k
              Q_l  = gsl_sf_legendre_Pl     (l,    mKK_mapped);
              P_i  = gsl_sf_legendre_Pl     (i,    ctheta_2);
              // only consider case where k >= 0
              // these are the real valued spherical harmonics
              if ( k == 0 ) Y_jk =           gsl_sf_legendre_sphPlm (j, k, ctheta_1);
              else          Y_jk = sqrt(2) * gsl_sf_legendre_sphPlm (j, k, ctheta_1) * cos(k*phi);
              acc_g += c[l][i][k][j]*(Q_l * P_i * Y_jk);
            }
/*Fill tree and show progress**************************************************/
    outtree->Fill();
    if(ievt%(nevt/100) == 0)
    {
      bar.print(ievt);
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
