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
#include "PeakingBackgroundFunctions.h"
void BsMassFit(string MCfilename, string REfilename, string SignalModel, string BackgroundModel, bool doSweight, string branchtofit, string plotfilename, bool drawpulls, int drawregion, string cuts, vector<string> backgrounds, vector<double> yields,bool logy,vector<string> yopts)
{
  using namespace std;
/*Input************************************************************************/
  // Open the input file and get the tree
  using namespace RooFit;
  cout << "Fitting to the branch " << branchtofit << endl;
  RooRealVar mass(branchtofit.c_str(),"m(K+K−K+K−) MeV",5200,5600);
/*Set up the fitter************************************************************/
  MassFitter phiKKFitter(&mass);
  RooRealVar* Nsig  = new RooRealVar("N","Number of signal events",4500,0,120000);
  RooRealVar* Nbkg  = new RooRealVar("N","Number of background events",1830,0,20000);
  Component* SigMod = phiKKFitter.AddComponent("Signal",SignalModel,Nsig);
  Component* BkgMod = phiKKFitter.AddComponent("Combinatorial",BackgroundModel,Nbkg);
  unsigned int npkbkgs = backgrounds.size();
  vector<Component*> PkgMod;
/*Peaking background fit*******************************************************/
  if(npkbkgs == yields.size() && npkbkgs > 0)
  {
    for(unsigned int i = 0; i < npkbkgs; i++)
    {
      string name = "peaking"+itoa(i);
      char ext[] = ".root";
      size_t extpos = backgrounds[i].find(ext);
      string PBfilename = backgrounds[i].substr(0,extpos+strlen(ext));
      RooRealVar* PBmass;
      size_t cbnpos = backgrounds[i].find("/",extpos);
      RooDataSet* PBdata;
      RooAbsPdf* PDFtoPlot;
      MassFitter* PBFitter;
      double N = yields[i];
      RooAbsReal* yield;
      string yopt = yopts.size() == npkbkgs? yopts[i] : "abs";
      if(yopt == "flo")
      {
        yield = new RooRealVar("N","",N,0,10000);
      }
      else if (yopt == "rel")
      {
        char formula[20];
        sprintf(formula,"@0*%f",N);
        yield = new RooFormulaVar("N",formula,RooArgList(*Nsig));
      }
      else
      {
        yield = new RooRealVar("N","",N);
      }
      string shapename = "Crystal Ball + 1 Gaussian";
      Component* comp;
      if(cbnpos != string::npos)
      {
        // Different branch name and range
        string PBbranch = backgrounds[i].substr(cbnpos+1);
        if(PBbranch=="HISTPDF")
        {
          PBmass = &mass;
          PBdata = PeakingBackgroundData(name,PBfilename,cuts,PBmass);
          PDFtoPlot = PeakingBackgroundHist(name,PBfilename,cuts,PBmass);
          comp = phiKKFitter.AddComponent(name,PDFtoPlot,yield);
          comp->SetStyle(kSolid);
        }
        else
        {
          PBmass = new RooRealVar(PBbranch.c_str(),"m(K+K−K+K−) MeV",5000,5600);
          PBdata = PeakingBackgroundData(name,PBfilename,cuts,PBmass);
          PBFitter = new MassFitter(PBmass);
          Component* PBMod = PBFitter->AddComponent(("temp"+name).c_str(),shapename);
          if(shapename == "Crystal Ball + 1 Gaussian")
          {
            PBMod->SetRange("mean",PBmass->getMin()-400,PBmass->getMax()+400);
            PBMod->SetValue("mean",PBdata->mean(*PBmass));
            PBMod->SetRange("sigma1",0,40);
            PBMod->SetRange("sigma2",0,100);
          }
          PBMod->FixShapeTo(PBdata);
          comp = phiKKFitter.AddComponent(name,shapename,yield);
          comp->FixShapeTo(*PBMod);
          PDFtoPlot = PBMod->GetPDF();
        }
      }
      else
      {
        PBmass = &mass;
        PBdata = PeakingBackgroundData(name,PBfilename,cuts,PBmass);
        comp = phiKKFitter.AddComponent(name,shapename,yield);
        if(shapename == "Crystal Ball + 1 Gaussian")
        {
          comp->SetRange("mean",PBmass->getMin(),PBmass->getMax());
          comp->SetValue("mean",PBdata->mean(*PBmass));
          comp->SetRange("sigma1",0,40);
          comp->SetRange("sigma2",0,100);
        }
        comp->FixShapeTo(PBdata);
        PDFtoPlot = comp->GetPDF();
      }
      PkgMod.push_back(comp);
      RooPlot* PBframe = PBmass->frame();
      PBdata->plotOn(PBframe,Binning(50));
      PDFtoPlot->plotOn(PBframe,LineStyle(kSolid),LineColor(kRed));
      cout << "Plotting peaking background" << endl;
      plotmaker* PBplotter;
      if(drawpulls)
      {
        RooHist* pullhist = PBframe->pullHist();
        RooPlot* pullframe = PBmass->frame(Title("Pull"));
        pullframe->addPlotable(pullhist,"B");
        PBplotter = new plotmaker(PBframe,pullframe);
      }
      else
      {
        PBplotter = new plotmaker(PBframe);
      }
      PBplotter->SetTitle("#it{m}(#it{#phi K^{#plus}K^{#minus}})", "MeV/#it{c}^{2}");
      TCanvas* canv = PBplotter->Draw();
      canv->SaveAs((plotfilename+"_PB"+itoa(i)+".pdf").c_str());
      if(PBmass != &mass)
      {
        delete PBmass;
        delete PBFitter;
      }
      delete PBplotter;
    }
  }
/*Monte Carlo fit**************************************************************/
  TFile* MCfile = new TFile(MCfilename.c_str());
  TTree* MCtree = GetTree(MCfile,cuts);
  RooDataSet MCdata("MCdata","\\phi \\phi \\text{ mass data}",mass,RooFit::Import(*MCtree));
  SigMod->FixShapeTo(&MCdata);
  RooPlot* MCframe = mass.frame();
  MCdata.plotOn(MCframe,Binning(50));
  SigMod->GetPDF()->plotOn(MCframe,LineStyle(kSolid),LineColor(kRed));
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
  TCanvas* MCcanv = MCplotter->Draw();
  MCcanv->SaveAs((plotfilename+"_MC.pdf").c_str());
/*Collision data fit***********************************************************/
  TFile* REfile = new TFile(REfilename.c_str());
  TTree* REtree = GetTree(REfile,cuts);
  RooDataSet REdata("REdata","\\phi \\phi \\text{ mass data}",RooArgSet(mass),RooFit::Import(*REtree));
  RooPlot* REframe = mass.frame();
  REdata.plotOn(REframe,Binning(50));
  double resolution = 0, f1, f2, s1, s2, s3;
  f1 = SigMod->GetValue("fgaus1");
  f2 = SigMod->GetValue("fgaus2");
  s1 = SigMod->GetValue("sigma1");
  s2 = SigMod->GetValue("sigma2");
  s3 = SigMod->GetValue("sigma3");
  resolution = sqrt(f1*s1*s1 + (1-f1)*(f2*s2*s2 + (1-f2)*s3*s3));
  // Free up the resolution scale factor
  SigMod->SetRange("scalef",0.9,1.1);
  SigMod->FloatPar("scalef");
  // Free up the mean of the Bs peak
  SigMod->SetValue("mean",5366.77);
  SigMod->FloatPar("mean");
  // Do the fit and plot the result
  phiKKFitter.Fit(&REdata);
  resolution *= SigMod->GetValue("scalef");
  phiKKFitter.Plot(REframe);
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
  double mean = SigMod->GetValue("mean");
  cout << "The mass (μ) from data is: " << mean << " MeV/c^2" << endl;
  cout << "The mass resolution (σ) in Monte Carlo is: " << resolution << " MeV/c^2" << endl;
  RooAbsPdf* sigmod = SigMod->GetPDF();
  RooAbsPdf* bkgmod = BkgMod->GetPDF();
  for(int window = 2; window <= 3; window++)
  {
      cout << "Integrating fitted data PDF over μ±" << window << "σ" << endl;
      mass.setRange((itoa(window)+"sigma").c_str(),mean-window*resolution,mean+window*resolution);
      RooAbsReal* sigmodint = sigmod->createIntegral(mass,NormSet(mass),Range((itoa(window)+"sigma").c_str()));
      cout << "S:\t" << sigmodint->getVal()*Nsig->getVal() << endl;
      RooAbsReal* bkgmodint = bkgmod->createIntegral(mass,NormSet(mass),Range((itoa(window)+"sigma").c_str()));
      cout << "B:\t" << bkgmodint->getVal()*Nbkg->getVal() << endl;
      for(unsigned int i = 0; i < npkbkgs; i++)
      {
        RooAbsReal* pkgmodint = PkgMod[i]->GetPDF()->createIntegral(mass,NormSet(mass),Range((itoa(window)+"sigma").c_str()));
        cout << "B" << i << ":\t" << pkgmodint->getVal()*PkgMod[i]->GetValue("N") << endl;
      }
  }
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
  // Save the plots
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
    RooStats::SPlot* sData = phiKKFitter.GetsPlot(Nsig,Nbkg);
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
       << "$\\alpha      $ & $" << SigMod->GetValue("alpha")  << " \\pm " << SigMod->GetError("alpha")  << "$ \\\\" << endl
       << "$n            $ & $" << SigMod->GetValue("n")      << " \\pm " << SigMod->GetError("n")      << "$ \\\\" << endl
       << "$\\sigma_1    $ & $" << SigMod->GetValue("sigma1") << " \\pm " << SigMod->GetError("sigma1") << "$ \\\\" << endl
       << "$\\sigma_2    $ & $" << SigMod->GetValue("sigma2") << " \\pm " << SigMod->GetError("sigma2") << "$ \\\\" << endl
       << "$\\sigma_3    $ & $" << SigMod->GetValue("sigma3") << " \\pm " << SigMod->GetError("sigma3") << "$ \\\\" << endl
       << "$f_1          $ & $" << SigMod->GetValue("fgaus1") << " \\pm " << SigMod->GetError("fgaus1") << "$ \\\\" << endl
       << "$f_2          $ & $" << SigMod->GetValue("fgaus2") << " \\pm " << SigMod->GetError("fgaus2") << "$ \\\\" << endl
       << "\\hline" << endl
       << "$\\mu         $ & $" << SigMod->GetValue("mean")   << " \\pm " << SigMod->GetError("mean")   << "$ \\\\" << endl
       << "$N_\\text{sig}$ & $" << SigMod->GetValue("N")      << " \\pm " << SigMod->GetError("N")      << "$ \\\\" << endl
       << "$N_\\text{bkg}$ & $" << BkgMod->GetValue("N")      << " \\pm " << BkgMod->GetError("N")      << "$ \\\\" << endl;
/******************************************************************************/
  return;
}
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  string MCfile, REfile, sigPDF, bkgPDF, plotname, branchname, cuts;
  vector<string> pkbkgs, yopts;
  vector<double> yields;
  int drawregion;
  desc.add_options()
    ("help,H"      ,                                                                             "produce help message"           )
    ("sweight,W"   ,                                                                             "apply sweights to data"         )
    ("draw-region" , value<int>(&drawregion   )->default_value(0                              ), "draw lines at ±Nσ"              )
    ("pulls,P"     ,                                                                             "plot with pulls"                )
    ("MCfile,M"    , value<string>(&MCfile    )->default_value("ntuples/BsphiKK_MC_mva.root"  ), "Monte Carlo file"               )
    ("REfile,R"    , value<string>(&REfile    )->default_value("ntuples/BsphiKK_data_mva.root"), "collision data file"            )
    ("sigPDF,S"    , value<string>(&sigPDF    )->default_value("Crystal Ball + 2 Gaussians"   ), "signal PDF to fit to data"      )
    ("bkgPDF,B"    , value<string>(&bkgPDF    )->default_value("Exponential"                  ), "background PDF to fit to data"  )
    ("plotname,O"  , value<string>(&plotname  )->default_value("BsphiKK_data"                 ), "fit plot filename"              )
    ("branchname,N", value<string>(&branchname)->default_value("B_s0_LOKI_Mass"               ), "branch to fit"                  )
    ("cuts,C"      , value<string>(&cuts      )->default_value(""                             ), "optional cuts"                  )
    ("backgrounds" , value<vector<string>>(&pkbkgs)->multitoken(                              ), "peaking background MC files"    )
    ("yields"      , value<vector<double>>(&yields)->multitoken(                              ), "background yields"              )
    ("yopts"       , value<vector<string>>(&yopts)->multitoken(                               ), "BG yield options: abs, rel, flo")
    ("logy"        ,                                                                             "log y scale"                    )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  BsMassFit(MCfile, REfile, sigPDF, bkgPDF, vmap.count("sweight"), branchname, plotname, vmap.count("pulls"), drawregion, cuts, pkbkgs, yields, vmap.count("logy"), yopts);
  return 0;
}
