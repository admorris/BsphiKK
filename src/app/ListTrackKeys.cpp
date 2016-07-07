#include "TFile.h"
#include "TTree.h"
#include <iostream>

int main(int argc, char* argv[])
{
  // Get the tree
  TFile* file = TFile::Open(argv[1]);
  TTree* tree = (TTree*) file->Get("DecayTree");
  // Variables to read
  ULong64_t evt; tree->SetBranchAddress("eventNumber"      , &evt);
  UInt_t    run; tree->SetBranchAddress("runNumber"        , &run);
  int      key1; tree->SetBranchAddress("Kminus_TRACK_Key" ,&key1);
  int      key2; tree->SetBranchAddress("Kminus0_TRACK_Key",&key2);
  int      key3; tree->SetBranchAddress("Kplus_TRACK_Key"  ,&key3);
  int      key4; tree->SetBranchAddress("Kplus0_TRACK_Key" ,&key4);
  // Event loop
  int num_entries = tree->GetEntries();
  for (int i = 0; i < num_entries; ++i)
  {
    tree->GetEntry(i);
    std::cout << evt << "\t" << run << "\t"<< key1 << "\t" << key2 << "\t" << key3 << "\t" << key4 << std::endl;
  }
  return 0;
}
