// Std
#include <iostream>
#include <string>
// ROOT
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
// RooFit
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
  const int maxpower[3] = {1,1,1}; // Actual max power is value-1
  // Observables
  RooRealVar obs[3] = {RooRealVar("Phi_angle","#Phi",-TMath::Pi(),+TMath::Pi())
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
        C[i][j][k] = RooRealVar(("C"+itoa(i)+itoa(j)+itoa(k)).c_str(),"",0,0,1000);
      }
    }
  }
  C[0][0][0] = RooRealVar("C000","",100,0,1000);
  // PDF
  RooArgList coefficients;
  RooArgList terms;
  RooGenericPdf* term;
  string LegendreTerms[3];
  for(int i = 0; i < maxpower[0]; i++)
  {
    LegendreTerms[0] = "ROOT::Math::legendre("+itoa(i)+",[0]/TMath::Pi())";
    for(int j = 0; j < maxpower[1]; j++)
    {
      LegendreTerms[1] = "ROOT::Math::legendre("+itoa(j)+",[1])";
      for(int k = 0; k < maxpower[2]; k++)
      {
        LegendreTerms[2] = "ROOT::Math::legendre("+itoa(k)+",[2])";
        coefficients.add(C[i][j][k]);
//        string expression = LegendreTerms[0]+"*"+LegendreTerms[1]+"*"+LegendreTerms[2];
        string expression = "@0*@1*@2*@3";
        term = new RooGenericPdf(("P"+itoa(i)+itoa(j)+itoa(k)).c_str(),expression.c_str(),RooArgList(C[i][j][k],obs[0],obs[1],obs[2]));
        terms.add(*term);
//        cout << C[i][j][k].GetName() << "*" << term->GetName() << endl;
        term->Print();
      }
    }
  }
//  RooAddPdf* model = new RooAddPdf("model","",terms);
  RooGenericPdf* model = term;
  cout << "Constructed PDF" << endl;
  // Input
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  RooDataSet data("data","",Observables,Import(*tree));
  cout << "Imported data" << endl;
  // Do fit
//  model->fitTo(data);
  // Plot data
  RooPlot* frame[3];
  for(int i = 0; i < 3; i++)
  {
    frame[i] = obs[i].frame();
    data.plotOn(frame[i]);
    model->plotOn(frame[i]);
    frame[i]->Draw();
    gPad->SaveAs(("projection"+itoa(i)+".pdf").c_str());
  }
  cout << "Fin." << endl;
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
