#include <string>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector2.h"
//#include  "ksfun.h"


/* 
*  Add the rapid fit variables to the standard ntuple. To use, type then root and at the prompt
*   .L AddRapidFitInfo.C++
*   AddRapidFitInfo("TheFile.root", "DecayTree")
*   It will write out TheFile_RapidFit.root
*/

#include <vector>
#include <algorithm>
#include "TMath.h"

class CloneInfo{

public:

  CloneInfo(int key1, int key2, int key3, int key4, int i) :
    m_key1(key1),
    m_key2(key2),
    m_key3(key3),
    m_key4(key4),
    m_i(i) ,  m_isAlive(true){}


  CloneInfo( const CloneInfo& cl) :
    m_key1(cl.m_key1),   m_key2(cl.m_key2) ,  m_key3(cl.m_key3) , m_key4(cl.m_key4) , m_i(cl.m_i), m_isAlive(cl.m_isAlive)
  {}


  CloneInfo() {}

  ~CloneInfo() {}

  bool isAlive() {return m_isAlive; }

  int i() const {return m_i;}

  //  double chi2() const {return m_chi2;}

  void setDead(){m_isAlive = false;}

  int key1(){ return m_key1 ;}

  int key2(){ return m_key2 ;}

  int key3(){ return m_key3 ;}

  int key4(){ return m_key4 ;}


  int sum() const {return m_key1 + m_key2 + m_key3 + m_key4  ;} 

  class Less_by_sum{
  public:
    inline bool operator() (const CloneInfo& c1, const CloneInfo& c2 ) const{
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

void tagClone(CloneInfo& c1, CloneInfo& c2){

  if (c1.sum() == c2.sum() ) {
    c2.setDead();
  }

}

void tagClones(std::vector<CloneInfo>& clones) {

  std::vector<CloneInfo>::iterator iter = clones.begin();
  for (;iter != clones.end(); ++iter){
    if (iter->isAlive() == true){
      std::vector<CloneInfo>::iterator iter2 = iter; ++iter2;
      for (;iter2 != clones.end(); ++iter2){
	if (iter2->sum() == iter->sum()) iter2->setDead();
      } // iter2
    } // iter
  }

}
void sortClones(std::vector<CloneInfo>& clones){
  std::sort(clones.begin(), clones.end(), CloneInfo::Less_by_sum());
}


void addToClones(std::vector<CloneInfo>& clones, int key1, int key2, int key3, int key4, int i ) {
  CloneInfo tclone = CloneInfo(key1, key2, key3, key4,  i);
  clones.push_back(tclone);
}


//static const int asize = 5e6;

std::string RemoveDup(std::string fileName = "BsphiKK_data_duplicates.root" , std::string treeName = "DecayTreeTuple/DecayTree"){

  // get the input
  TChain* tree = new TChain(treeName.c_str());
  tree->Add(fileName.c_str());
  std::cout << "Read in the tree " << std::endl;

  //  tree->Print();


  // clone the tree..
  int num_entries  = tree->GetEntries();

  std::vector<int> flag_clones; flag_clones.resize(num_entries);
  for (int i =0; i < num_entries; ++i){
    flag_clones[i] = 1;
  }


  // some vertex chi2 branch
  double dvchi2;
  tree->SetBranchAddress("B0_ENDVERTEX_CHI2",&dvchi2); 

  UInt_t run;
  tree->SetBranchAddress("runNumber",&run);  
  
  ULong64_t event;
  tree->SetBranchAddress("eventNumber",&event);  

  int key1;
  tree->SetBranchAddress("Kminus_TRACK_Key",&key1);  
  int key2;
  tree->SetBranchAddress("Kminus0_TRACK_Key",&key2);  
  int key3;
  tree->SetBranchAddress("Kplus_TRACK_Key",&key3);  
  int key4;
  tree->SetBranchAddress("Kplus0_TRACK_Key",&key4);  


  
  std::vector<CloneInfo> clones;

  int i  =0;
  while (i <num_entries) {
    tree->GetEntry(i);  

    int run_i = run;
    int event_i = event;

    clones.clear();  addToClones(clones,key1,key2,key3,key4, i);

    //    std::cout << " here " << key1  << " " << key2 << " " << key3 << " "<< key4 << std::endl;
    int j = i+1;
    for (j=i+1;j<num_entries; j++) {
      tree->GetEntry(j);
      int run_j = run;
      int event_j = event;
      if(run_j!=run_i||event_j!=event_i) {
        break;
      }
      else {
	//	std::cout << key1  << " " << key2 << " " << key3 << " "<< key4 << std::endl;
        addToClones(clones, key1,key2,key3,key4, j);   
      }
    } //j
    sortClones(clones);
    tagClones(clones);
    
    if (clones.size() > 1) {
      std::cout << clones.size() << std::endl;
      for (unsigned int ii = 0; ii < clones.size() ; ++ii){
        flag_clones[clones[ii].i()] = clones[ii].isAlive();
        std::cout << ii << " "  <<  clones[ii].i() << " " << clones[ii].isAlive() << " " << clones[ii].key1() << 
	  " " <<  clones[ii].key2() << " " << clones[ii].key3() <<  " " << clones[ii].key4()<< std::endl;
      }
      std::cout << "***" << std::endl;
    }
    
    i= j;

  } // i
  

  // make the output
  std::string outputName = fileName.substr(0,fileName.size() - 5);
  outputName += "_Clone.root";
  TFile* outFile  =new TFile(outputName.c_str(),"RECREATE");
  std::cout << "Reading: " << treeName << " from " << fileName  << " to " << outputName << std::endl;
   

  TTree*  newtree = tree->CloneTree(-1);
  int isAlive;
  TBranch *branch_bestVtxChi2 = newtree->Branch("isDup",&isAlive, "isDup/I");
  num_entries = newtree->GetEntries(); 
  

  // loop again and write the branch
  for ( i=0;i<num_entries; i++) {
    newtree->GetEntry(i); 
    isAlive = flag_clones[i];
    branch_bestVtxChi2->Fill();
  }
 
  newtree->Write();
  outFile->Close();
 
  return outputName;
}

void FlagClones()
{
  RemoveDup("BsphiKK_data_duplicates.root");
  RemoveDup("BsphiKK_MC_duplicates.root");
  RemoveDup("Bsphiphi_MC_duplicates.root");
  return;
}
