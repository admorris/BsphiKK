
#include <string>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <TMath.h> 
#include <iostream.h>
#include <sstream>
//#include "JStyle.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
using namespace RooFit;


float minMass = 5000;
float maxMass = 5500;

void plot()
{

  TFile* f = new TFile("withbdt.root");
  TTree* tree = (TTree*) f->Get("SelTree");
  int num_entries  = tree->GetEntries();

  TCut cuts = "bdt >0.0 && TMath::Abs(m_kstar-892)< 90 && TMath::Abs(m_phi-1020)<20" ;

   TH1F* histo = new TH1F("histo","histo", 200, 5000, 5500.);

    /*
  float mass;tree->SetBranchAddress("B_s0_M",&mass);
  TH1F* histo = new TH1F("mass","mass", 100, 5000, 5500.);

  double sumSig = 0;
  std::cout << "Start to loop over newtree " << num_entries <<  std::endl;
  for (int i = 0; i <num_entries; ++i) {
    tree->GetEntry(i);
    histo->Fill(mass);
    } */

   tree->Draw("mass>>histo",cuts) ;

   histo->Draw(); 
   
}

// ar alpha =  ( d1.Mag2() - d2.Mag2() ) / ( d1 + d2 ).Mag2()
