#include <iostream>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooRealVar.h"
#include "RooUniform.h"

#include "progbar.h"

using namespace std;
using namespace RooFit;

TH1D* normalise(TH1D* hist)
{
  hist->Scale(1.0/hist->Integral());
  return hist;
}

TH1D* toyhist(RooAbsPdf* PDF, RooRealVar* x, int nbins = 100)
{
  RooDataSet* data = PDF->generate(*x,1000000);
  string name = (string)PDF->GetName()+"Hist";
  RooDataHist* hist = data->binnedClone();
  return (TH1D*)hist->createHistogram(name.c_str(),*x,Binning(nbins));
}
void AngularAcceptance(string filename)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  TH1D* PhiHist = new TH1D("PhiHist","",100,-3.142,3.142);
  TH1D* ct1Hist = new TH1D("ct1Hist","",100,-1,1);
  TH1D* ct2Hist = new TH1D("ct2Hist","",100,-1,1);
  double Phi_angle ; tree->SetBranchAddress("Phi_angle", &Phi_angle );
  double cos_theta1; tree->SetBranchAddress("cos_theta1",&cos_theta1);
  double cos_theta2; tree->SetBranchAddress("cos_theta2",&cos_theta2);
  RooRealVar Phi("PhiVar","",0,-3.142,3.142);
  RooRealVar ct1("ct1Var","",0,-1,1);
  RooRealVar ct2("ct2Var","",0,-1,1);
  RooUniform* PhiPDF = new RooUniform("Phi","",Phi);
  RooUniform* ct1PDF = new RooUniform("ct1","",ct1);
  RooUniform* ct2PDF = new RooUniform("ct2","",ct2);
  unsigned int n = tree->GetEntries();
  cout << "Reading in angles from MC." << endl;
  progbar bar(n);
  for(unsigned int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    PhiHist->Fill(Phi_angle );
    ct1Hist->Fill(cos_theta1);
    ct2Hist->Fill(cos_theta2);
    if(i%100==0)
    {
      bar.print(i);
    }
  }
  bar.terminate();
  normalise(PhiHist);
  normalise(ct1Hist);
  normalise(ct2Hist);
  PhiHist->Divide(normalise(toyhist(PhiPDF,&Phi)));
  ct1Hist->Divide(normalise(toyhist(ct1PDF,&ct1)));
  ct2Hist->Divide(normalise(toyhist(ct2PDF,&ct2)));
  TFile* outputfile = new TFile("AngAcc.root","RECREATE");
  outputfile->cd();
  PhiHist->Write();
  ct1Hist->Write();
  ct2Hist->Write();
  file->Close();
  outputfile->Close();
}

int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1]);
  return 0;
}
