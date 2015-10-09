// Standard headers
#include <stdexcept>
#include <iostream>
#include <string>
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// RooFit headers
#include "RooAbsPdf.h"
#include "RooRealVar.h"
// Custom headers
#include "MassFitter.h"
void BsMassFit(string filename = "BsphiKK_MC")
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
  RooRealVar mass("B_s0_M","#font[132]{#it{m}(#it{K^{#plus}K^{#minus}K^{#plus}K^{#minus}}) [MeV/}#font[12]{c}#font[132]{^{2}}]",5150,5600);
  RooDataSet data("data","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*tree));
  MassFitter FitModel(mass);
  FitModel.SetPDF("Single Gaussian");
  FitModel.Fit(&data);
  FitModel.SetPDF("Double Gaussian");
  FitModel.Fit(&data);
  FitModel.SetPDF("Triple Gaussian");
  FitModel.Fit(&data);
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