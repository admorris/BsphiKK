#include <string>
#include <iostream>
#include "TBranch.h"
#include "TMath.h"
#include <vector>
#include <stdexcept>
#include "TMath.h"
#include "CloneInfo.h"
#include "CloneTagger.h"
#include "GetTree.h"
using namespace std;
void FlagClones(string fileName = "BsphiKK_data_duplicates.root")
{
  // get the input
  TTree* tree = GetTree(fileName.c_str());
  // clone the tree..
  int num_entries = tree->GetEntries();
  vector<int> flag_clones; flag_clones.resize(num_entries);
  for (int i = 0; i < num_entries; ++i)
  {
    flag_clones[i] = 1;
  }
  double dvchi2; tree->SetBranchAddress("B_s0_ENDVERTEX_CHI2",&dvchi2);
  UInt_t run;tree->SetBranchAddress("runNumber",&run);
  ULong64_t event; tree->SetBranchAddress("eventNumber",&event);
  int key1; tree->SetBranchAddress("Kminus_TRACK_Key",&key1);
  int key2; tree->SetBranchAddress("Kminus0_TRACK_Key",&key2);
  int key3; tree->SetBranchAddress("Kplus_TRACK_Key",&key3);
  int key4; tree->SetBranchAddress("Kplus0_TRACK_Key",&key4);
  int i = 0;
  cout << "Finding duplicate candidates in a sample containing " << num_entries << " events" << endl;
  while(i < num_entries)
  {
    tree->GetEntry(i);
    int run_i = run;
    int event_i = event;
    vector<CloneInfo> clones;
    CloneTagger tagger(clones);
    tagger.addToClones({key1,key2,key3,key4},i,dvchi2);
    int j = i+1;
    for (j=i+1;j<num_entries; j++)
    {
      tree->GetEntry(j);
      int run_j = run;
      int event_j = event;
      if(run_j!=run_i || event_j!=event_i)
      {
        break;
      }
      else
      {
        tagger.addToClones({key1,key2,key3,key4},j,dvchi2);
      }
    }
    tagger.sortClones();
    tagger.tagClones();
    if(clones.size() > 1)
    {
      for(auto clone : clones)
      {
        flag_clones[clone.i()] = clone.isAlive();
      }
    }
    i=j;
  }
  // make the output
  string outputName = fileName.substr(0,fileName.size() - 5);
  outputName += "_Clone.root";
  TFile* outFile = TFile::Open(outputName.c_str(),"RECREATE");
  cout << "Reading: " << tree->GetName() << " from " << fileName  << " to " << outputName << endl;
  TTree*  newtree = tree->CloneTree(-1);
  int isAlive;
  TBranch *branch_bestVtxChi2 = newtree->Branch("isDup",&isAlive, "isDup/I");
  num_entries = newtree->GetEntries();
  // loop again and write the branch
  for (i=0;i<num_entries; i++)
  {
    newtree->GetEntry(i);
    isAlive = flag_clones[i];
    branch_bestVtxChi2->Fill();
  }
  newtree->Write();
  outFile->Close();
  return;
}
int main(int argc, char* argv[])
{
  if(argc!=2)
  {
    cout << "Usage: " << argv[0] << " <file name>" << endl;
    return 1;
  }
  FlagClones((string)argv[1]);
  return 0;
}
