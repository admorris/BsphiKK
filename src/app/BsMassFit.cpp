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
#include "TLine.h"
// RooFit headers
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
// RooStats headers
#include "RooStats/SPlot.h"
// Custom headers
#include "MassFitter.h"
#include "progbar.h"
#include "plotmaker.h"
#include "GetTree.h"
#include "itoa.h"
RooHistPdf* PeakingBackgroundHist(string name, string filename, string cuts, RooRealVar* massvar)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = GetTree(file,cuts);
  RooDataSet* data = new RooDataSet((name+"data").c_str(),"",*massvar,RooFit::Import(*tree));
  RooDataHist* hist = new RooDataHist((name+"hist").c_str(),"",*massvar,*data);
  return new RooHistPdf(name.c_str(),"",*massvar,*hist);
}
RooDataSet* PeakingBackgroundData(string name, string filename, string cuts, RooRealVar* massvar)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = GetTree(file,cuts);
  return new RooDataSet((name+"data").c_str(),"",*massvar,RooFit::Import(*tree));
}
void BsMassFit(string MCfilename, string REfilename, string SignalModel, string BackgroundModel, bool doSweight, string branchtofit, string plotfilename, bool drawpulls, int drawregion, string cuts, vector<string> backgrounds, vector<double> yields,bool logy)
{
  using namespace std;
/*Input************************************************************************/
  // Open the input file and get the tree
  using namespace RooFit;
  cout << "Fitting to the branch " << branchtofit << endl;
  RooRealVar mass(branchtofit.c_str(),"m(K+K−K+K−) MeV",5200,5600);
/*Set up the fitter************************************************************/
  MassFitter REFitModel(&mass);
  RooRealVar* Nsig  = new RooRealVar("N","Number of signal events",4500,0,120000);
  RooRealVar* Nbkg  = new RooRealVar("N","Number of background events",1830,0,20000);
  Component* RESigMod = REFitModel.AddComponent("Signal",SignalModel,Nsig);
  Component* REBkgMod = REFitModel.AddComponent("Combinatorial",BackgroundModel,Nbkg);
  unsigned int npkbkgs = backgrounds.size();
  if(npkbkgs == yields.size() && npkbkgs > 0)
  {
    for(unsigned int i = 0; i < npkbkgs; i++)
    {
      string name = "peaking"+itoa(i);
      double N = yields[i];
      RooRealVar* yield = N>0? new RooRealVar("N","",N) : new RooRealVar("N","",0,0,10000);
      Component* comp = REFitModel.AddComponent(name,"Crystal Ball",yield);
      RooDataSet* PBdata = PeakingBackgroundData(name,backgrounds[i],cuts,&mass);
      comp->SetRange("mean",mass.getMin(),mass.getMax());
      comp->SetValue("mean",PBdata->mean(mass));
      comp->FixShapeTo(PBdata);
    }
  }
/*Monte Carlo fit**************************************************************/
  TFile* MCfile = new TFile(MCfilename.c_str());
  TTree* MCtree = GetTree(MCfile,cuts);
  RooDataSet MCdata("MCdata","\\phi \\phi \\text{ mass data}",mass,RooFit::Import(*MCtree));
  RESigMod->FixShapeTo(&MCdata);
/*Collision data fit***********************************************************/
  TFile* REfile = new TFile(REfilename.c_str());
  TTree* REtree = GetTree(REfile,cuts);
  RooDataSet REdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*REtree));
  RooPlot* REframe = mass.frame();
  REdata.plotOn(REframe,Binning(50));
  double resolution = 0, f1, f2, s1, s2, s3;
  f1 = RESigMod->GetValue("fgaus1");
  f2 = RESigMod->GetValue("fgaus2");
  s1 = RESigMod->GetValue("sigma1");
  s2 = RESigMod->GetValue("sigma2");
  s3 = RESigMod->GetValue("sigma3");
  resolution = sqrt(f1*s1*s1 + (1-f1)*(f2*s2*s2 + (1-f2)*s3*s3));
  RESigMod->SetRange("scalef",0.9,1.1);
  RESigMod->FloatPar("scalef");
  RESigMod->SetValue("mean",5366.77);
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
  TCanvas* canv = REplotter->Draw(logy);
/*Output S and B for MC optimisation*******************************************/
  double mean = RESigMod->GetValue("mean");
  cout << "The mass (μ) from data is: " << mean << " MeV/c^2" << endl;
  cout << "The mass resolution (σ) in Monte Carlo is: " << resolution << " MeV/c^2" << endl;
  RooAbsPdf* sigmod = RESigMod->GetPDF();
  RooAbsPdf* bkgmod = REBkgMod->GetPDF();
  cout << "Integrating fitted data PDF over μ±2σ" << endl;
  mass.setRange("twosigma",mean-2*resolution,mean+2*resolution);
  RooAbsReal* sigmodint2 = sigmod->createIntegral(mass,NormSet(mass),Range("twosigma"));
  cout << "S:\t" << sigmodint2->getVal()*Nsig->getVal() << endl;
  RooAbsReal* bkgmodint2 = bkgmod->createIntegral(mass,NormSet(mass),Range("twosigma"));
  cout << "B:\t" << bkgmodint2->getVal()*Nbkg->getVal() << endl;
  cout << "Integrating fitted data PDF over μ±3σ" << endl;
  mass.setRange("threesigma",mean-3*resolution,mean+3*resolution);
  RooAbsReal* sigmodint3 = sigmod->createIntegral(mass,NormSet(mass),Range("threesigma"));
  cout << "S:\t" << sigmodint3->getVal()*Nsig->getVal() << endl;
  RooAbsReal* bkgmodint3 = bkgmod->createIntegral(mass,NormSet(mass),Range("threesigma"));
  cout << "B:\t" << bkgmodint3->getVal()*Nbkg->getVal() << endl;
  if(drawregion!=0)
  {
    cout << "Drawing lines" << endl;
//    canv->cd(0);
    TLine* hiline = new TLine(mean+drawregion*resolution,0,mean+drawregion*resolution,REframe->GetMaximum());
    TLine* loline = new TLine(mean-drawregion*resolution,0,mean-drawregion*resolution,REframe->GetMaximum());
//    hiline->SetLineColor(2);
//    loline->SetLineColor(2);
    hiline->SetLineStyle(2);
    loline->SetLineStyle(2);
    hiline->Draw();
    loline->Draw();
  }
  canv->SaveAs((plotfilename+".pdf").c_str());
  canv->SaveAs((plotfilename+".root").c_str());
/*S-weight the given ntuple****************************************************/
  if(doSweight)
  {
    using namespace RooStats;
    string trailer = "_Sweighted.root";
    string outputName = ((string)REfile->GetName()).substr(0, ((string)REfile->GetName()).size() - 5) + trailer;
    TFile* outputFile = new TFile(outputName.c_str(),"RECREATE");
    TTree* newtree = REtree->CloneTree(-1);
    cout << "copied the tree" << endl;
    RooStats::SPlot* sData = REFitModel.GetsPlot(Nsig,Nbkg);
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
  cout //<< setprecision(2)
       << "$\\alpha      $ & $" << RESigMod->GetValue("alpha")  << " \\pm " << RESigMod->GetError("alpha")  << "$ \\\\" << endl
       << "$n            $ & $" << RESigMod->GetValue("n")      << " \\pm " << RESigMod->GetError("n")      << "$ \\\\" << endl
       << "$\\sigma_1    $ & $" << RESigMod->GetValue("sigma1") << " \\pm " << RESigMod->GetError("sigma1") << "$ \\\\" << endl
       << "$\\sigma_2    $ & $" << RESigMod->GetValue("sigma2") << " \\pm " << RESigMod->GetError("sigma2") << "$ \\\\" << endl
       << "$\\sigma_3    $ & $" << RESigMod->GetValue("sigma3") << " \\pm " << RESigMod->GetError("sigma3") << "$ \\\\" << endl
       << "$f_1          $ & $" << RESigMod->GetValue("fgaus1") << " \\pm " << RESigMod->GetError("fgaus1") << "$ \\\\" << endl
       << "$f_2          $ & $" << RESigMod->GetValue("fgaus2") << " \\pm " << RESigMod->GetError("fgaus2") << "$ \\\\" << endl
       << "\\hline" << endl
       << "$\\mu         $ & $" << RESigMod->GetValue("mean")   << " \\pm " << RESigMod->GetError("mean")   << "$ \\\\" << endl
       << "$N_\\text{sig}$ & $" << RESigMod->GetValue("N")      << " \\pm " << RESigMod->GetError("N")      << "$ \\\\" << endl
       << "$N_\\text{bkg}$ & $" << REBkgMod->GetValue("N")      << " \\pm " << REBkgMod->GetError("N")      << "$ \\\\" << endl;
/******************************************************************************/
  return;
}
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  string MCfile, REfile, sigPDF, bkgPDF, plotname, branchname, cuts;
  vector<string> pkbkgs;
  vector<double> yields;
  int drawregion;
  desc.add_options()
    ("help,H"      ,                                                                             "produce help message"              )
    ("sweight,W"   ,                                                                             "apply sweights to data"            )
    ("draw-region" , value<int>(&drawregion   )->default_value(0                              ), "draw lines at ±Nσ"                 )
    ("pulls,P"     ,                                                                             "plot with pulls"                   )
    ("MCfile,M"    , value<string>(&MCfile    )->default_value("ntuples/BsphiKK_MC_mva.root"  ), "Monte Carlo file"                  )
    ("REfile,R"    , value<string>(&REfile    )->default_value("ntuples/BsphiKK_data_mva.root"), "collision data file"               )
    ("sigPDF,S"    , value<string>(&sigPDF    )->default_value("Crystal Ball + 2 Gaussians"   ), "signal PDF to fit to data"         )
    ("bkgPDF,B"    , value<string>(&bkgPDF    )->default_value("Exponential"                  ), "background PDF to fit to data"     )
    ("plotname,O"  , value<string>(&plotname  )->default_value("BsphiKK_data"                 ), "fit plot filename"                 )
    ("branchname,N", value<string>(&branchname)->default_value("B_s0_LOKI_Mass"               ), "branch to fit"                     )
    ("cuts,C"      , value<string>(&cuts      )->default_value(""                             ), "optional cuts"                     )
    ("backgrounds" , value<vector<string>>(&pkbkgs)->multitoken(                              ), "peaking background MC files"       )
    ("yields"      , value<vector<double>>(&yields)->multitoken(                              ), "background yields, set <0 to float")
    ("logy"        ,                                                                             "log y scale"                       )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  BsMassFit(MCfile, REfile, sigPDF, bkgPDF, vmap.count("sweight"), branchname, plotname, vmap.count("pulls"), drawregion, cuts, pkbkgs, yields, vmap.count("logy"));
  return 0;
}
