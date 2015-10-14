#include <vector>
#include <numeric>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TChain.h"
#include "TH1F.h"
#include <iostream>
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include <math.h>
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooVoigtian.h"
#include "RooBackPdf.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooGaussian.h"

//#include "ksfun.h"

// binding 7.34 5.35 87.1

//gSystem->load("libRooFit");
using namespace RooFit;


double minMass = 3600.0;
double maxMass = 4000.0;
int nbin = 200;

void readX(){

  TFile *_file0 = TFile::Open("withbdt.root");
  TTree* tree = gFile->Get("SelTree;1");

  TH1F* histo = new TH1F("m", "m", nbin, minMass, maxMass); histo->Sumw2();
  TH1F* bhisto = new TH1F("bm", "bm", nbin, minMass, maxMass); bhisto->Sumw2();
 

  float tzPsi; 
  tree->SetBranchAddress("tzPsi",&tzPsi);
  
  float m;
  tree->SetBranchAddress("mass",&m);
  float bdt;
  tree->SetBranchAddress("bdt",&bdt);

 
  int nevent = tree->GetEntries(); 
  int n = 0;

  for ( int i = 0; i < nevent; ++i){
    tree->GetEntry(i);
    histo->Fill(m);
    //cout << tzPsi << endl;
    if (bdt> -0.6&& tzPsi > 0.3) bhisto->Fill(m);
  }

  histo->SetMinimum(0); 
 histo->Draw();
 //bhisto->Divide(histo); 
 bhisto->Draw("HISTOSAME");
 }   


  

  



