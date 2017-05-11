// Standard headers
#include <stdexcept>
#include <iostream>
#include <string>
#include <algorithm>
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
#include "plotmaker.h"
#include "GetTree.h"
#include "GetData.h"
#include "ResultDB.h"
void BsMassFit(string MCfilename, string CDfilename, string SignalModel, string BackgroundModel, bool doSweight, string branchtofit, string plotfilename, bool drawpulls, int drawregion, string cuts, vector<string> backgrounds, vector<double> yields,bool logy,vector<string> yopts, string resname, string DBfilename, bool noblurb)
{
  int nbins = 50;
  using namespace std;
/*Input************************************************************************/
  // Open the input file and get the tree
  using namespace RooFit;
  cout << "Fitting to the branch " << branchtofit << endl;
  RooRealVar mass(branchtofit.c_str(),"#it{m}(#it{#phi K^{#plus}K^{#minus}}) [MeV/#it{c}^{2}]",5150,5600);
/*Set up the fitter************************************************************/
  MassFitter phiKKFitter(&mass);
  RooRealVar* Nsig  = new RooRealVar("N","Number of signal events",4500,0,120000);
  RooRealVar* Nbkg  = new RooRealVar("N","Number of background events",1830,0,20000);
  RooArgList yieldlist(*Nsig,*Nbkg);
  bool sWarning = false; // Because printing a warning during the peaking background loop just gets buried.
  Component* SigMod = phiKKFitter.AddComponent("Signal",SignalModel,Nsig);
  SigMod->SetColour(4);
  SigMod->SetStyle (9);
  Component* BkgMod = phiKKFitter.AddComponent("Combinatorial",BackgroundModel,Nbkg);
  BkgMod->SetColour(6);
  BkgMod->SetStyle (3);
  //int builtinstyles = 3;
  int linecolors[] = {8, 28, 1};
  int linestyles[] = {1,  2, 5};
  //int builtinstyles = sizeof(linecolors)/sizeof(int);
  assert(sizeof(linecolors)==sizeof(linestyles));
  unsigned int npkbkgs = backgrounds.size();
  vector<Component*> PkgMod;
/*Peaking background fit*******************************************************/
  if(npkbkgs == yields.size() && npkbkgs > 0)
  {
    for(unsigned int i = 0; i < npkbkgs; i++)
    {
      string name = "peaking"+std::to_string(i);
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
      yieldlist.add(*yield);
      if(doSweight && yopt != "flo") sWarning = true;
      string shapename = "Crystal Ball + 1 Gaussian";
      Component* comp;
      if(cbnpos != string::npos)
      {
        // Different branch name and range
        string PBbranch = backgrounds[i].substr(cbnpos+1);
        if(PBbranch=="HISTPDF")
        {
          PBmass = &mass;
          PBdata = GetData(name,PBfilename,"",PBmass);
          PDFtoPlot = GetDataHist(name,PBfilename,"",PBmass);
          comp = phiKKFitter.AddComponent(name,PDFtoPlot,yield);
          comp->SetStyle(kSolid);
        }
        else
        {
          PBmass = new RooRealVar(PBbranch.c_str(),"#it{m}(#it{#phi K^{#plus}K^{#minus}}) [MeV/#it{c}^{2}]",5000,5600);
          PBdata = GetData(name,PBfilename,"",PBmass);
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
        PBdata = GetData(name,PBfilename,"",PBmass);
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
      PBdata->plotOn(PBframe,Binning(nbins));
      PDFtoPlot->plotOn(PBframe,LineStyle(kSolid),LineColor(kRed));
      cout << "Plotting peaking background" << endl;
      plotmaker* PBplotter;
      if(drawpulls)
      {
        RooHist* pullhist = PBframe->pullHist();
        RooPlot* pullframe = PBmass->frame(Title("Pull"));
        pullframe->addPlotable(pullhist,"B");
        PBplotter = new plotmaker(PBframe);
        PBplotter->SetPullPlot(pullframe);
      }
      else
      {
        PBplotter = new plotmaker(PBframe);
      }
      if(noblurb) PBplotter->SetBlurb("");
      PBplotter->SetTitle("#it{m}(#it{#phi K^{#plus}K^{#minus}})", "MeV/#it{c}^{2}");
      TCanvas* canv = PBplotter->Draw();
      canv->SaveAs((plotfilename+"_PB"+std::to_string(i)+".pdf").c_str());
      if(PBmass != &mass)
      {
        delete PBmass;
        delete PBFitter;
      }
      delete PBplotter;
      comp->SetColour(linecolors[i]);
      comp->SetStyle (linestyles[i]);
    }
  }
/*Monte Carlo fit**************************************************************/
  TTree* MCtree = GetTree(MCfilename,cuts);
  RooDataSet MCdata("MCdata","",mass,RooFit::Import(*MCtree));
  SigMod->FixShapeTo(&MCdata);
  RooPlot* MCframe = mass.frame();
  MCdata.plotOn(MCframe,Binning(nbins));
  SigMod->GetPDF()->plotOn(MCframe,LineStyle(kSolid),LineColor(kRed));
  plotmaker* MCplotter;
  if(drawpulls)
  {
    RooHist* pullhist = MCframe->pullHist();
    RooPlot* pullframe = mass.frame(Title("Pull"));
    pullframe->addPlotable(pullhist,"B");
    MCplotter = new plotmaker(MCframe);
    MCplotter->SetPullPlot(pullframe);
  }
  else
  {
    MCplotter = new plotmaker(MCframe);
  }
  if(noblurb) MCplotter->SetBlurb("");
  MCplotter->SetTitle("#it{m}(#it{#phi K^{#plus}K^{#minus}})", "MeV/#it{c}^{2}");
  TCanvas* MCcanv = MCplotter->Draw();
  MCcanv->SaveAs((plotfilename+"_MC.pdf").c_str());
/*Collision data fit***********************************************************/
  TTree* CDtree = GetTree(CDfilename,cuts);
  RooDataSet CDdata("CDdata","",RooArgSet(mass),RooFit::Import(*CDtree));
  RooPlot* CDframe = mass.frame();
  CDdata.plotOn(CDframe,Binning(nbins));
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
  phiKKFitter.Fit(&CDdata);
  resolution *= SigMod->GetValue("scalef");
  phiKKFitter.Plot(CDframe);
  plotmaker* CDplotter;
  if(drawpulls)
  {
    RooHist* pullhist = CDframe->pullHist();
    RooPlot* pullframe = mass.frame(Title("Pull"));
    pullframe->addPlotable(pullhist,"B");
    CDplotter = new plotmaker(CDframe);
    CDplotter->SetPullPlot(pullframe);
  }
  else
  {
    CDplotter = new plotmaker(CDframe);
  }
  if(noblurb) CDplotter->SetBlurb("");
  CDplotter->SetTitle("#it{m}(#it{#phi K^{#plus}K^{#minus}})", "MeV/#it{c}^{2}");
  CDplotter->SetLogy(logy);
  TCanvas* canv = CDplotter->Draw();
/*Output S and B for MC optimisation*******************************************/
  double mean = SigMod->GetValue("mean");
  cout << "The mass (μ) from data is: " << mean << " MeV/c^2" << endl;
  cout << "The mass resolution (σ) in Monte Carlo is: " << resolution << " MeV/c^2" << endl;
  RooAbsPdf* sigmod = SigMod->GetPDF();
  RooAbsPdf* bkgmod = BkgMod->GetPDF();
  double Nsigtwosigma   , Nsigtwosigmaerr
       , Nsigthreesigma , Nsigthreesigmaerr
       , Nbkgtwosigma   , Nbkgtwosigmaerr
       , Nbkgthreesigma , Nbkgthreesigmaerr
       ;
  for(int window = 2; window <= 3; window++)
  {
    cout << "Integrating fitted data PDF over μ±" << window << "σ" << endl;
    mass.setRange((std::to_string(window)+"sigma").c_str(),mean-window*resolution,mean+window*resolution);
    RooAbsReal* sigmodint = sigmod->createIntegral(mass,NormSet(mass),Range((std::to_string(window)+"sigma").c_str()));
    RooAbsReal* bkgmodint = bkgmod->createIntegral(mass,NormSet(mass),Range((std::to_string(window)+"sigma").c_str()));
    double tempNsig    = sigmodint->getVal()*Nsig->getVal()
         , tempNsigerr = sigmodint->getVal()*Nsig->getError()
         , tempNbkg    = bkgmodint->getVal()*Nbkg->getVal()
         , tempNbkgerr = bkgmodint->getVal()*Nbkg->getError()
         ;
    if(window == 2)
    {
      Nsigtwosigma    = tempNsig;
      Nsigtwosigmaerr = tempNsigerr;
      Nbkgtwosigma    = tempNbkg;
      Nbkgtwosigmaerr = tempNbkgerr;
    }
    else if(window == 3)
    {
      Nsigthreesigma    = tempNsig;
      Nsigthreesigmaerr = tempNsigerr;
      Nbkgthreesigma    = tempNbkg;
      Nbkgthreesigmaerr = tempNbkgerr;
    }
    cout << "S:\t" << tempNsig << "±" << tempNsigerr << endl;
    cout << "B:\t" << tempNbkg << "±" << tempNbkgerr << endl;
    for(unsigned int i = 0; i < npkbkgs; i++)
    {
      RooAbsReal* pkgmodint = PkgMod[i]->GetPDF()->createIntegral(mass,NormSet(mass),Range((std::to_string(window)+"sigma").c_str()));
      cout << "B" << i << ":\t" << pkgmodint->getVal()*PkgMod[i]->GetValue("N") << endl;
    }
  }
  if(drawregion!=0)
  {
    cout << "Drawing lines" << endl;
//    canv->cd(0);
    TLine* hiline = new TLine(mean+drawregion*resolution,0,mean+drawregion*resolution,CDframe->GetMaximum());
    TLine* loline = new TLine(mean-drawregion*resolution,0,mean-drawregion*resolution,CDframe->GetMaximum());
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
    string outputName = CDfilename.substr(0, CDfilename.size() - 5) + trailer;
    TFile* outputFile = TFile::Open(outputName.c_str(),"RECREATE");
    outputFile->cd();
    TTree* newtree = CDtree->CloneTree(0);
    cout << "copied the tree" << endl;
    RooStats::SPlot* sData = phiKKFitter.GetsPlot(yieldlist);
    sData->GetName(); // Just to prevent compiler warning
    float Nsig_sw; newtree->Branch("Nsig_sw", &Nsig_sw,"Nsig_sw/F");
    float Nbkg_sw; newtree->Branch("Nbkg_sw", &Nbkg_sw,"Nbkg_sw/F");
    float L_Nsig;  newtree->Branch("L_Nsig",  &L_Nsig, "L_Nsig/F" );
    float L_Nbkg;  newtree->Branch("L_Nbkg",  &L_Nbkg, "L_Nbkg/F" );
    for (int i = 0; i < CDdata.numEntries(); i++)
    {
      CDtree->GetEntry(i);
      const RooArgSet* row = CDdata.get(i);
      Nsig_sw =  row->getRealValue("SignalN_sw");
      L_Nsig  =  row->getRealValue("L_SignalN" );
      Nbkg_sw =  row->getRealValue("CombinatorialN_sw");
      L_Nbkg  =  row->getRealValue("L_CombinatorialN" );
      newtree->Fill();
      if((i%1000)==0) cout << i << "/" << CDdata.numEntries() << endl;
    }
    cout << "Done" << endl;
    newtree->Write();
    outputFile->Close();
    cout << "Written to " << outputFile->GetName() << endl;
    if(sWarning) cout << "WARNING: sWeights will not be calculated properly because a peaking background yield is not floated." << endl;
  }
  vector<parameter> pars;
  pars.push_back(parameter("alpha" ,"\\alpha"  ,SigMod));
  pars.push_back(parameter("n"     ,"n"        ,SigMod));
  pars.push_back(parameter("sigma1","\\sigma_1",SigMod));
  pars.push_back(parameter("sigma2","\\sigma_2",SigMod));
  pars.push_back(parameter("sigma3","\\sigma_3",SigMod));
  pars.push_back(parameter("fgaus1","f_1"      ,SigMod));
  pars.push_back(parameter("fgaus2","f_2"      ,SigMod));
  pars.push_back(parameter("mean"  ,"\\mu"     ,SigMod));
  parameter Nsigpar("N","N_\\text{sig}",SigMod); pars.push_back(Nsigpar);
  parameter Nbkgpar("N","N_\\text{bkg}",BkgMod); pars.push_back(Nbkgpar);
/******************************************************************************/
  if(resname!="")
  {
    ResultDB table(DBfilename);
    for(auto par : pars)
    {
      table.Update(resname+par.safename(),"N",par.value,par.error);
    }
    table.Update(resname+Nsigpar.safename()+"twosigma"  ,"N",Nsigtwosigma  ,Nsigtwosigmaerr  );
    table.Update(resname+Nsigpar.safename()+"threesigma","N",Nsigthreesigma,Nsigthreesigmaerr);
    table.Update(resname+Nbkgpar.safename()+"twosigma"  ,"N",Nbkgtwosigma  ,Nbkgtwosigmaerr  );
    table.Update(resname+Nbkgpar.safename()+"threesigma","N",Nbkgthreesigma,Nbkgthreesigmaerr);
    table.Save();
    cout << "Results saved to " << DBfilename << endl;
  }
  for(auto par : pars)
  {
    cout << "$" << par.latex << "$ & $" << par.value << " \\pm " << par.error << "$ \\\\" << endl;
  }
/******************************************************************************/
  return;
}
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  string MCfile, CDfile, sigPDF, bkgPDF, plotname, branchname, cuts, resname, dbf;
  vector<string> pkbkgs, yopts;
  vector<double> yields;
  int drawregion;
  desc.add_options()
    ("help,H"      ,                                                                             "produce help message"           )
    ("noblurb"     ,                                                                             "no blurb"                       )
    ("sweight,W"   ,                                                                             "apply sweights to data"         )
    ("pulls,P"     ,                                                                             "plot with pulls"                )
    ("logy"        ,                                                                             "log y scale"                    )
    ("draw-region" , value<int>(&drawregion   )->default_value(0                              ), "draw lines at ±Nσ"              )
    ("MCfile,M"    , value<string>(&MCfile    )->default_value("ntuples/BsphiKK_MC_mva.root"  ), "Monte Carlo file"               )
    ("CDfile,R"    , value<string>(&CDfile    )->default_value("ntuples/BsphiKK_data_mva.root"), "collision data file"            )
    ("sigPDF,S"    , value<string>(&sigPDF    )->default_value("Crystal Ball + 2 Gaussians"   ), "signal PDF to fit to data"      )
    ("bkgPDF,B"    , value<string>(&bkgPDF    )->default_value("Exponential"                  ), "background PDF to fit to data"  )
    ("plotname,O"  , value<string>(&plotname  )->default_value("BsphiKK_data"                 ), "fit plot filename"              )
    ("branchname,N", value<string>(&branchname)->default_value("B_s0_LOKI_Mass"               ), "branch to fit"                  )
    ("cuts,C"      , value<string>(&cuts      )->default_value(""                             ), "optional cuts"                  )
    ("backgrounds" , value<vector<string>>(&pkbkgs)->multitoken(                              ), "peaking background MC files"    )
    ("yields"      , value<vector<double>>(&yields)->multitoken(                              ), "background yields"              )
    ("yopts"       , value<vector<string>>(&yopts )->multitoken(                              ), "BG yield options: abs, rel, flo")
    ("output-file" , value<string>(&dbf       )->default_value("FitResults.csv"               ), "output file"                    )
    ("save-results", value<string>(&resname   )->default_value(""                             ), "name to save results as"        )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  BsMassFit(MCfile, CDfile, sigPDF, bkgPDF, vmap.count("sweight"), branchname, plotname, vmap.count("pulls"), drawregion, cuts, pkbkgs, yields, vmap.count("logy"), yopts, resname, dbf,vmap.count("noblurb"));
  return 0;
}

