// Standard headers
#include <stdexcept>
#include <iostream>
#include <string>
// ROOT headers
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
// RooFit headers
#include "RooAbsPdf.h"
#include "RooRealVar.h"
// Custom headers
#include "MassFitter.h"
void BsMassFit(string filename)
{
  using namespace std;
  cout << "Fitting the Bs mass in " << filename << endl;
/*Input************************************************************************/
  // Open the input file and get the tree
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
/*Do the fit*******************************************************************/
  cout << "Importing tree." << endl;
  using namespace RooFit;
  RooRealVar mass("B_s0_LOKI_Mass","#font[132]{#it{m}(#it{K^{#plus}K^{#minus}K^{#plus}K^{#minus}}) [MeV/}#font[12]{c}#font[132]{^{2}}]",5250,5500);
  RooDataSet data("data","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*tree));
  RooPlot* frame = mass.frame();
  data.plotOn(frame);
  MassFitter FitModel(&mass);
  FitModel.SetPDF("Triple Gaussian","Flat");
  FitModel.Fit(&data);
  FitModel.Plot(frame);
  frame->Draw();
  gPad->SaveAs("test.pdf");

/******************************************************************************/
  return;
}
int main(int argc, char* argv[])
{
  using namespace std;
  if(argc==1)
  {
    throw invalid_argument("Please provide a file name.");
  }
  else if(argc>2)
  {
    throw invalid_argument("Too many arguments.");
  }
  BsMassFit((string)argv[1]);
  return 0;
}
