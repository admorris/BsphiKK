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
// void BsMassFit(string filename)
void BsMassFit()
{
  using namespace std;
/*Input************************************************************************/
  // Open the input file and get the tree
  using namespace RooFit;
  RooRealVar mass("B_s0_LOKI_Mass","#font[132]{#it{m}(#it{K^{#plus}K^{#minus}K^{#plus}K^{#minus}}) [MeV/}#font[12]{c}#font[132]{^{2}}]",5250,5500);
/*Monte Carlo fit**************************************************************/
  TFile* MCfile = new TFile("ntuples/BsphiKK_MC_mvaVars_vetoes.root");
  TTree* MCtree = (TTree*)MCfile->Get("DecayTree");
  RooDataSet MCdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*MCtree));
  RooPlot* MCframe = mass.frame();
  MCdata.plotOn(MCframe);
  MassFitter MCFitModel(&mass);
  MCFitModel.SetPDF("Triple Gaussian","none");
  MCFitModel.Fit(&MCdata);
  MCFitModel.Plot(MCframe);
  MCframe->Draw();
  gPad->SaveAs("testMC.pdf");
/*Collision data fit***********************************************************/
  TFile* REfile = new TFile("ntuples/BsphiKK_data_mvaVars_vetoes.root");
  TTree* REtree = (TTree*)REfile->Get("DecayTree");
  RooDataSet REdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*REtree));
  RooPlot* REframe = mass.frame();
  REdata.plotOn(REframe);
  MassFitter REFitModel(&mass);
  REFitModel.SetPDF("Triple Gaussian","Exponential");
  string fixme[] =
  {
    "sigma1"
  , "sigma2"
  , "sigma3"
  , "fgaus1"
  , "fgaus2"
  };
  for(unsigned int i = 0; i < sizeof(fixme)/sizeof(string); i++)
  {
    REFitModel.FixValue(fixme[i],MCFitModel.GetValue(fixme[i]));
  }
  REFitModel.Fit(&REdata);
  REFitModel.Plot(REframe);
  REframe->Draw();
  gPad->SaveAs("testRE.pdf");
/******************************************************************************/
  return;
}
int main(int argc, char* argv[])
{
  /*
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
  */
  BsMassFit();
  return 0;
}
