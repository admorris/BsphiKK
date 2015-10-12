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
// RooStats headers
#include "RooStats/SPlot.h"
// Custom headers
#include "MassFitter.h"
#include "progbar.h"
// void BsMassFit(string filename)
void BsMassFit(string ModelName = "Crystal Ball + 2 Gaussians", bool doSweight = false)
{
  using namespace std;
/*Input************************************************************************/
  // Open the input file and get the tree
  using namespace RooFit;
  double upperlimit, lowerlimit;
  if(doSweight)
  {
    lowerlimit = 5000;
    upperlimit = 6000;
  }
  else
  {
    lowerlimit = 5200;
    upperlimit = 5600;
  }
  cout << "Fitting in the range " << lowerlimit << " MeV/c^2 to " << upperlimit << "MeV/c^2" << endl;
  RooRealVar mass("B_s0_LOKI_Mass","#font[132]{#it{m}(#it{K^{#plus}K^{#minus}K^{#plus}K^{#minus}}) [MeV/}#font[12]{c}#font[132]{^{2}}]",lowerlimit,upperlimit);
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
/*Output S and B for MC optimisation*******************************************/
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
/*S-weight the given ntuple****************************************************/
  if(doSweight)
  {
    using namespace RooStats;
    string trailer = "_Sweighted.root";
    string outputName = ((string)REfile->GetName()).substr(0, ((string)REfile->GetName()).size() - 5) + trailer;
    TFile* outputFile = new TFile(outputName.c_str(),"RECREATE");
    TTree* newtree = REtree->CloneTree(-1);
    cout << "copied the tree" << endl;
    RooStats::SPlot* sData = REFitModel.GetsPlot();
    float Nsig_sw; TBranch*  b_Nsig_sw = newtree->Branch("Nsig_sw", &Nsig_sw,"Nsig_sw/F");
    float Nbkg_sw; TBranch*  b_Nbkg_sw = newtree->Branch("Nbkg_sw", &Nbkg_sw,"Nbkg_sw/F");
    float L_Nsig;  TBranch*  b_L_Nsig  = newtree->Branch("L_Nsig",  &L_Nsig, "L_Nsig/F" );
    float L_Nbkg;  TBranch*  b_L_Nbkg  = newtree->Branch("L_Nbkg",  &L_Nbkg, "L_Nbkg/F" );
    progbar bar(REdata.numEntries());
    for (int i = 0; i < REdata.numEntries(); ++i)
    {
      newtree->GetEntry(i);
      const RooArgSet* row = REdata.get(i);
      Nsig_sw =  row->getRealValue("Nsig_sw");
      L_Nsig  =  row->getRealValue("L_Nsig" );
      Nbkg_sw =  row->getRealValue("Nbkg_sw");
      L_Nbkg  =  row->getRealValue("L_Nbkg" );
      if (i % 10)
        bar.print(i);
      b_Nsig_sw->Fill();
      b_L_Nsig->Fill();
      b_Nbkg_sw->Fill();
      b_L_Nbkg->Fill();
    }
    bar.terminate();
    newtree->Write();
    outputFile->Close();
  }
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
  else if(argc>3)
  {
    throw invalid_argument("Too many arguments.");
  }
  BsMassFit((string)argv[1],(string)argv[2]=="sweight");
  return 0;
}
