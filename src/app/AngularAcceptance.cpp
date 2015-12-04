// Std
#include <iostream>
#include <string>
// ROOT
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
// RooFit
#include "RooLegendre.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooGenericPdf.h"
// Common
#include "progbar.h"
#include "itoa.h"

using namespace std;
using namespace RooFit;
void AngularAcceptance(string filename)
{
  // Configuration
  const int maxpower[3] = {1,1,2}; // Actual max power is value-1
  // Observables
  RooRealVar obs[3] = {RooRealVar("cos_Phi"   ,"cos #Phi"      ,-1,1)
                      ,RooRealVar("cos_theta1","cos #theta_{1}",-1,1)
                      ,RooRealVar("cos_theta2","cos #theta_{2}",-1,1)};
  RooArgSet Observables;
  for(int i = 0; i < 3; i++)
  {
    Observables.add(obs[i]);
  }
  // Coefficients
  RooRealVar C[maxpower[0]][maxpower[1]][maxpower[2]];
  for(int i = 0; i < maxpower[0]; i++)
  {
    for(int j = 0; j < maxpower[1]; j++)
    {
      for(int k = 0; k < maxpower[2]; k++)
      {
        C[i][j][k] = RooRealVar(("C"+itoa(i)+itoa(j)+itoa(k)).c_str(),"",0,-1,1);
      }
    }
  }
  // PDF
  RooArgList coefficients;
  RooArgList terms;
  RooLegendre* P[3];
  RooGenericPdf* term;
  for(int i = 0; i < 3; i++)
  {
    P[i] = new RooLegendre[maxpower[i]];
    for(int j = 0; j < maxpower[i]; j++)
    {
      P[i][j] = RooLegendre(("P"+itoa(j)+"("+obs[i].GetName()+")").c_str(),("P_{"+itoa(j)+"}("+obs[i].GetTitle()+")").c_str(),obs[i],j);
    }
  }
  for(int i = 0; i < maxpower[0]; i++)
  {
    for(int j = 0; j < maxpower[1]; j++)
    {
      for(int k = 0; k < maxpower[2]; k++)
      {
        coefficients.add(C[i][j][k]);
        term = new RooGenericPdf(("P"+itoa(i)+itoa(j)+itoa(k)).c_str(),"@0*@1*@2",RooArgList(P[0][i],P[1][i],P[2][i]));
        terms.add(*term);
      }
    }
  }
  RooAddPdf model("mode","",terms,coefficients);
  cout << "Constructed PDF" << endl;
  // Input
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  RooDataSet data("data","",Observables,Import(*tree));
  // Do fit
  model.fitTo(data);
  // Plot data
  RooPlot* frame[3];
  for(int i = 0; i < 3; i++)
  {
    frame[i] = obs[i].frame();
    data.plotOn(frame[i]);
    model.plotOn(frame[i]);
    frame[i]->Draw();
    gPad->SaveAs(("projection"+itoa(i)+".pdf").c_str());
  }
}

int main(int argc, char* argv[])
{
  if(argc!=2)
  {
    cout << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }
  AngularAcceptance(argv[1]);
  return 0;
}
