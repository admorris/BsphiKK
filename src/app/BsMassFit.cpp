// Standard headers
#include <stdexcept>
#include <iostream>
#include <string>
// BOOST headers
#include "boost/program_options.hpp"
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
#include "plotmaker.h"
// void BsMassFit(string filename)
void BsMassFit(string MCfilename, string REfilename, string SignalModel, string BackgroundModel, bool doSweight, string branchtofit, string plotfilename, bool drawpulls)
{
  using namespace std;
/*Input************************************************************************/
  // Open the input file and get the tree
  using namespace RooFit;
  cout << "Fitting to the branch " << branchtofit << endl;
  RooRealVar mass(branchtofit.c_str(),"m(K+K−K+K−) MeV",5200,5600);
/*Monte Carlo fit**************************************************************/
  TFile* MCfile = new TFile(MCfilename.c_str());
  TTree* MCtree = ((TTree*)MCfile->Get("DecayTree"));
  RooDataSet MCdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*MCtree));
  RooPlot* MCframe = mass.frame();
  MCdata.plotOn(MCframe,Binning(50));
  MassFitter MCFitModel(&mass);
  MCFitModel.SetPDF(SignalModel,"none");
  MCFitModel.Fit(&MCdata);
  MCFitModel.Plot(MCframe);
  plotmaker* MCplotter;
  if(drawpulls)
  {
    RooHist* pullhist = MCframe->pullHist();
    RooPlot* pullframe = mass.frame(Title("Pull"));
    pullframe->addPlotable(pullhist,"B");
    MCplotter = new plotmaker(MCframe,pullframe);
  }
  else
  {
    MCplotter = new plotmaker(MCframe);
  }
  MCplotter->SetTitle("#it{m}(#it{#phi K^{#plus}K^{#minus}})", "MeV/#it{c}^{2}");
  MCplotter->Draw()->SaveAs((plotfilename+"_MC.pdf").c_str());
  // This bit is really horrible, sorry.
  double resolution = 0, f1, f2, s1, s2, s3;
  if (SignalModel == "Single Gaussian" || SignalModel == "Crystal Ball")
  {
    resolution = MCFitModel.GetValue("sigma1");
  }
  if (SignalModel == "Double Gaussian" || SignalModel == "Crystal Ball + 1 Gaussian")
  {
    f1 = MCFitModel.GetValue("fgaus1");
    s1 = MCFitModel.GetValue("sigma1");
    s2 = MCFitModel.GetValue("sigma2");
    resolution = sqrt(f1*s1*s1 + (1-f1)*s2*s2);
  }
  if (SignalModel == "Triple Gaussian" || SignalModel == "Crystal Ball + 2 Gaussians")
  {
    f1 = MCFitModel.GetValue("fgaus1");
    f2 = MCFitModel.GetValue("fgaus2");
    s1 = MCFitModel.GetValue("sigma1");
    s2 = MCFitModel.GetValue("sigma2");
    s3 = MCFitModel.GetValue("sigma3");
    resolution = sqrt(f1*s1*s1 + (1-f1)*(f2*s2*s2 + (1-f2)*s3*s3));
  }
/*Collision data fit***********************************************************/
  TFile* REfile = new TFile(REfilename.c_str());
  TTree* REtree = (TTree*)REfile->Get("DecayTree");
  RooDataSet REdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*REtree));
  RooPlot* REframe = mass.frame();
  REdata.plotOn(REframe,Binning(50));
  MassFitter REFitModel(&mass);
  REFitModel.SetPDF(SignalModel,BackgroundModel);
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
  plotmaker* REplotter;
  if(drawpulls)
  {
    RooHist* pullhist = REframe->pullHist();
    RooPlot* pullframe = mass.frame(Title("Pull"));
    pullframe->addPlotable(pullhist,"B");
    REplotter = new plotmaker(REframe,pullframe);
  }
  else
  {
    REplotter = new plotmaker(REframe);
  }
  REplotter->SetTitle("#it{m}(#it{#phi K^{#plus}K^{#minus}})", "MeV/#it{c}^{2}");
  REplotter->Draw()->SaveAs((plotfilename+".pdf").c_str());
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
    sData->GetName(); // Just to prevent compiler warning
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
      if (i % 100)
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
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string MCfile, REfile, sigPDF, bkgPDF, plotname, branchname;
  desc.add_options()
    ("help,H"      ,                                                                                  "produce help message"         )
    ("sweight,W"   ,                                                                                  "apply sweights to data"       )
    ("pulls,P"     ,                                                                                  "plot with pulls"              )
    ("MCfile,M"    , value<std::string>(&MCfile    )->default_value("ntuples/BsphiKK_MC_mva.root")  , "set Monte Carlo file"         )
    ("REfile,R"    , value<std::string>(&REfile    )->default_value("ntuples/BsphiKK_data_mva.root"), "set collision data file"      )
    ("sigPDF,S"    , value<std::string>(&sigPDF    )->default_value("Crystal Ball + 2 Gaussians")   , "signal PDF to fit to data"    )
    ("bkgPDF,B"    , value<std::string>(&bkgPDF    )->default_value("Exponential")                  , "background PDF to fit to data")
    ("plotname,O"  , value<std::string>(&plotname  )->default_value("BsphiKK_data")                 , "fit plot filename"            )
    ("branchname,N", value<std::string>(&branchname)->default_value("B_s0_LOKI_Mass")               , "branch to fit"                )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  BsMassFit(MCfile, REfile, sigPDF, bkgPDF, vmap.count("sweight"), branchname, plotname, vmap.count("pulls"));
  return 0;
}
