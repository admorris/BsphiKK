#include <string>
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include <vector>
#include <algorithm>
#include "TMath.h"
#include "progbar.h"
#include "CloneInfo.h"
#include "CloneTagger.h"
using namespace std;
void FlagClones(string fileName = "BsphiKK_data_duplicates.root" , string treeName = "DecayTreeTuple/DecayTree")
//  gSystem->Load("libprogbar.so");
  // get the input
  TChain* tree = new TChain(treeName.c_str());
  tree->Add(fileName.c_str());
  // clone the tree..
  int num_entries  = tree->GetEntries();
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
  vector<CloneInfo> clones;
  CloneTagger tagger(clones);
  int i = 0;
  cout << "Finding duplicate events" << endl;
  progbar bar(num_entries);
  while (i < num_entries)
  {
    tree->GetEntry(i);
    int run_i = run;
    int event_i = event;
    clones.clear();
    tagger.addToClones(key1,key2,key3,key4,i);
    int j = i+1;
    for (j=i+1;j<num_entries; j++)
    {
      tree->GetEntry(j);
      int run_j = run;
      int event_j = event;
        if(run_j!=run_i||event_j!=event_i)
        {
          break;
        }
        else
        {
          tagger.addToClones(key1,key2,key3,key4,j);
        }
    }
    tagger.sortClones();
    tagger.tagClones();
    if (clones.size() > 1)
    {
      for (unsigned int ii = 0; ii < clones.size() ; ++ii)
      {
        flag_clones[clones[ii].i()] = clones[ii].isAlive();
      }
    }
    i = j;
    if(i%100==0)
    {
      bar.print(i);
    }
  }
  bar.terminate();
  // make the output
  string outputName = fileName.substr(0,fileName.size() - 5);
  outputName += "_Clone.root";
  TFile* outFile  =new TFile(outputName.c_str(),"RECREATE");
  cout << "Reading: " << treeName << " from " << fileName  << " to " << outputName << endl;
  TTree*  newtree = tree->CloneTree(-1);
  int isAlive;
  TBranch *branch_bestVtxChi2 = newtree->Branch("isDup",&isAlive, "isDup/I");
  num_entries = newtree->GetEntries();
  // loop again and write the branch
  bar.reset();
  for (i=0;i<num_entries; i++)
  {
    newtree->GetEntry(i);
    isAlive = flag_clones[i];
    branch_bestVtxChi2->Fill();
    if(i%100==0)
    {
      bar.print(i);
    }
  }
  bar.terminate();
  newtree->Write();
  outFile->Close();
  return;
}
int main(int argc, char* argv[])
{
  if(argc!=3)
  {
    cout << "Usage: " << argv[0] << " <file name> <tree path/tree name>" << endl;
    return 1;
  }
  FlagClones((string)argv[1], (string)argv[2]);
  return 0;
}
