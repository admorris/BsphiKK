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
#include "../progbar.h"
using namespace std;
class CloneInfo
{
public:
  CloneInfo(int key1, int key2, int key3, int key4, int i) :
  m_key1(key1),
  m_key2(key2),
  m_key3(key3),
  m_key4(key4),
  m_i(i),
  m_isAlive(true){}
  CloneInfo(const CloneInfo& cl) :
  m_key1(cl.m_key1),
  m_key2(cl.m_key2),
  m_key3(cl.m_key3),
  m_key4(cl.m_key4),
  m_i(cl.m_i),
  m_isAlive(cl.m_isAlive){}
  CloneInfo() {}
  ~CloneInfo() {}
  bool isAlive() { return m_isAlive; }
  int i() const { return m_i; }
  void setDead(){m_isAlive = false;}
  int key1(){ return m_key1; }
  int key2(){ return m_key2; }
  int key3(){ return m_key3; }
  int key4(){ return m_key4; }
  int sum() const { return m_key1 + m_key2 + m_key3 + m_key4; } 
  class Less_by_sum
  {
    public:
      inline bool operator() (const CloneInfo& c1, const CloneInfo& c2) const
      {
        return c1.sum() < c2.sum();
      }
  };
private:
  int m_key1;
  int m_key2;
  int m_key3;
  int m_key4;
  int m_i;
  bool m_isAlive;
};
#ifdef __MAKECINT__
#pragma link C++ class vector<CloneInfo>;
#endif
void tagClone(CloneInfo& c1, CloneInfo& c2)
{
  if (c1.sum() == c2.sum())
  {
    c2.setDead();
  }
}
void tagClones(vector<CloneInfo>& clones)
{
  vector<CloneInfo>::iterator iter = clones.begin();
  for (;iter != clones.end(); ++iter)
  {
    if (iter->isAlive() == true)
    {
    vector<CloneInfo>::iterator iter2 = iter; ++iter2;
      for (;iter2 != clones.end(); ++iter2)
      {
	      if (iter2->sum() == iter->sum())
	      {
        	iter2->setDead();
	      }
      }
    }
  }
}
void sortClones(vector<CloneInfo>& clones)
{
  sort(clones.begin(), clones.end(), CloneInfo::Less_by_sum());
}
void addToClones(vector<CloneInfo>& clones, int key1, int key2, int key3, int key4, int i )
{
  CloneInfo tclone = CloneInfo(key1, key2, key3, key4,  i);
  clones.push_back(tclone);
}
string FlagClones(string fileName = "BsphiKK_data_duplicates.root" , string treeName = "DecayTreeTuple/DecayTree")
{
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
  int i = 0;
  cout << "Finding duplicate events" << endl;
  progbar bar(num_entries);
  while (i < num_entries)
  {
    tree->GetEntry(i);
    int run_i = run;
    int event_i = event;
    clones.clear();  addToClones(clones,key1,key2,key3,key4,i);
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
          addToClones(clones,key1,key2,key3,key4,j);  
        }
    }
    sortClones(clones);
    tagClones(clones);
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
  return outputName;
}
