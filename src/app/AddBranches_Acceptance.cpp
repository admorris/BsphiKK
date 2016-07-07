// Std headers
#include <string.h>
#include <iostream>
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// Custom headers
#include "FourDHist_Adaptive.h"
#include "GetTree.h"
using namespace std;
void addBranches(string acceptancefilename = "Acceptance.root", string inputfilename = "BsphiKK_data_mvacut.root", string outputfilename = "BsphiKK_data_mvacut_acceptance.root")
{
/*Load acceptance**************************************************************/
  cout << "Loading acceptance histogram from " << acceptancefilename << endl;
  TFile* accfile = TFile::Open(acceptancefilename.c_str());
/*Input************************************************************************/
  cout << "Reading from " << inputfilename << endl;
  // Open the input file and create the output file
  TFile* infile  = TFile::Open(inputfilename.c_str()),
       * outfile = TFile::Open(outputfilename.c_str(),"RECREATE");
  // Get the input tree and create an empty output tree
  TTree* intree  = GetTree(infile);
  outfile->cd();
  TTree* outtree = intree->CloneTree(0);
  // Read the number of events in the input file
  Int_t n = intree->GetEntries();
/*Variables for new branches***************************************************/
  double acceptance; outtree->Branch("acceptance",&acceptance,"acceptance/D");
/*Event loop*******************************************************************/
  for(Int_t i = 0; i < n; i++)
  {
    intree->GetEntry(i);
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
  if(argc == 4)
  {
    addBranches((string)argv[1],(string)argv[2],(string)argv[3]);
    return 0;
  }
  else
  {
    cerr << "Usage: " << argv[0] << " <acceptance file> <input file> <output file>" << endl;
    return 1;
  }
}
