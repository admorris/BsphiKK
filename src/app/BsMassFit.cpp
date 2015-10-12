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
void BsMassFit(string ModelName = "Crystal Ball + 2 Gaussians")
{
  using namespace std;
/*Input************************************************************************/
  // Open the input file and get the tree
  using namespace RooFit;
  RooRealVar mass("B_s0_LOKI_Mass","#font[132]{#it{m}(#it{K^{#plus}K^{#minus}K^{#plus}K^{#minus}}) [MeV/}#font[12]{c}#font[132]{^{2}}]",5200,5600);
/*Monte Carlo fit**************************************************************/
  TFile* MCfile = new TFile("ntuples/BsphiKK_MC_mvaVars_vetoes.root");
  TTree* MCtree = ((TTree*)MCfile->Get("DecayTree"));
  RooDataSet MCdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*MCtree));
  RooPlot* MCframe = mass.frame();
  MCdata.plotOn(MCframe);
  MassFitter MCFitModel(&mass);
  MCFitModel.SetPDF(ModelName,"none");
  MCFitModel.Fit(&MCdata);
  MCFitModel.Plot(MCframe);
  MCframe->Draw();
  gPad->SetLogy();
  gPad->SaveAs("testMC.pdf");
  // This bit is really horrible, sorry.
  double resolution = 0, f1, f2, s1, s2, s3;
  if (ModelName == "Single Gaussian" || ModelName == "Crystal Ball")
  {
    resolution = MCFitModel.GetValue("sigma1");
  }
  if (ModelName == "Double Gaussian" || ModelName == "Crystal Ball + 1 Gaussian")
  {
    f1 = MCFitModel.GetValue("fgaus1");
    s1 = MCFitModel.GetValue("sigma1");
    s2 = MCFitModel.GetValue("sigma2");
    resolution = sqrt(f1*s1*s1 + (1-f1)*s2*s2);
  }
  if (ModelName == "Triple Gaussian" || ModelName == "Crystal Ball + 2 Gaussians")
  {
    f1 = MCFitModel.GetValue("fgaus1");
    f2 = MCFitModel.GetValue("fgaus2");
    s1 = MCFitModel.GetValue("sigma1");
    s2 = MCFitModel.GetValue("sigma2");
    s3 = MCFitModel.GetValue("sigma3");
    resolution = sqrt(f1*s1*s1 + (1-f1)*(f2*s2*s2 + (1-f2)*s3*s3));
  }
/*Collision data fit***********************************************************/
  TFile* REfile = new TFile("ntuples/BsphiKK_data_mvaVars_vetoes.root");
  TTree* REtree = (TTree*)REfile->Get("DecayTree");
  RooDataSet REdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*REtree));
  RooPlot* REframe = mass.frame();
  REdata.plotOn(REframe);
  MassFitter REFitModel(&mass);
  REFitModel.SetPDF(ModelName,"Exponential");
  string fixme[] =
  {
    "sigma1"
  , "sigma2"
  , "sigma3"
  , "fgaus1"
  , "fgaus2"
  , "alpha"
  , "n"
  };
  for(unsigned int i = 0; i < sizeof(fixme)/sizeof(string); i++)
  {
    try
    {
      REFitModel.FixValue(fixme[i],MCFitModel.GetValue(fixme[i]));
    }
    catch(exception& e)
    {
      cout << e.what() << endl;
      continue;
    }
  }
  REFitModel.Fit(&REdata);
  REFitModel.Plot(REframe);
  REframe->Draw();
  gPad->SaveAs("testRE.pdf");
/******************************************************************************/
  double mean = REFitModel.GetValue("mean");
  double Nsig = REFitModel.GetValue("Nsig");
  double Nbkg = REFitModel.GetValue("Nbkg");
  cout << "The mass (μ) from data is: " << mean << " MeV/c^2" << endl;
  cout << "The mass resolution (σ) in Monte Carlo is: " << resolution << " MeV/c^2" << endl;
  RooAbsPdf* sigmod = (RooAbsPdf*)REFitModel.GetThing("sigmod");
  RooAbsPdf* bkgmod = (RooAbsPdf*)REFitModel.GetThing("bkgmod");
  cout << "Integrating fitted data PDF over μ±2σ" << endl;
  mass.setRange("twosigma",mean-2*resolution,mean+2*resolution);
  RooAbsReal* sigmodint2 = sigmod->createIntegral(mass,NormSet(mass),Range("twosigma"));
  cout << "S:\t" << sigmodint2->getValV()*Nsig << endl;
  RooAbsReal* bkgmodint2 = bkgmod->createIntegral(mass,NormSet(mass),Range("twosigma"));
  cout << "B:\t" << bkgmodint2->getValV()*Nbkg << endl;
  cout << "Integrating fitted data PDF over μ±3σ" << endl;
  mass.setRange("threesigma",mean-3*resolution,mean+3*resolution);
  RooAbsReal* sigmodint3 = sigmod->createIntegral(mass,NormSet(mass),Range("threesigma"));
  cout << "S:\t" << sigmodint3->getValV()*Nsig << endl;
  RooAbsReal* bkgmodint3 = bkgmod->createIntegral(mass,NormSet(mass),Range("threesigma"));
  cout << "B:\t" << bkgmodint3->getValV()*Nbkg << endl;
/******************************************************************************/
  return;
}
int main(int argc, char* argv[])
{
  using namespace std;
  if(argc==1)
  {
    cout << "Fitting with default PDF" << endl;
    BsMassFit();
    return 0;
  }
  else if(argc>2)
  {
    throw invalid_argument("Too many arguments.");
  }
  BsMassFit((string)argv[1]);
  return 0;
}
