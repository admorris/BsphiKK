// Standard headers
#include <string>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// RooFit headers
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooRealVar.h"
// Custom headers
#include "plotmaker.h"
#include "MassFitter.h"
#include "GetTree.h"
#include "GetData.h"
#include "ResultDB.h"

void FitLb(string filename, string Sfilename, string Bfilename, string branchname, string modelname, string xtitle, string unit, string plotname, string cuts, double xlow, double xup, int nbins, bool drawpulls,std::string blurb, bool doSweight, std::string resname, std::string DBfilename)
{
  using namespace RooFit;
  RooRealVar* x = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
  std::cout << "Importing trees" << endl;
  TTree* tree = GetTree(filename,cuts);
  RooDataSet* Cdata = GetData("collision",tree,x)
           ,* Sdata = GetData("Lb",Sfilename,cuts,x)
//           ,* Bdata = GetData("Bs",Bfilename,cuts,x)
           ;
  std::cout << "Done" << endl;
  RooPlot* Cframe = x->frame()
        ,* Sframe = x->frame()
//        ,* Bframe = x->frame()
        ;
  MassFitter FitModel(x);
  plotmaker<RooPlot>* plotter;
  RooRealVar NLb("N","",100,0,10000);
  RooRealVar NBs("N","",100,0,100000);
/*Fit to LbphiKp MC************************************************************/
  Component* SigMod = FitModel.AddComponent("Lb",modelname,&NLb);
  SigMod->SetRange("mean",xlow,xup);
  SigMod->SetValue("mean",5619.5);
  SigMod->FixShapeTo(Sdata);
  Sdata->plotOn(Sframe,Binning(nbins));
  SigMod->GetPDF()->plotOn(Sframe);
  if(drawpulls)
  {
    RooPlot* pullframe = x->frame(Title("Pull"));
    pullframe->addPlotable(Sframe->pullHist(),"B");
    plotter = new plotmaker<RooPlot>(Sframe);
    plotter->SetPullPlot(pullframe);
  }
  else
  {
    plotter = new plotmaker<RooPlot>(Sframe);
  }
  plotter->SetBlurb(blurb);
  plotter->SetTitle(xtitle, unit);
  plotter->Draw()->SaveAs((plotname+"S.pdf").c_str());
//  SigMod->FloatPar("mean");
/*Fit to BsphiKK MC************************************************************/
  Component* BkgMod = FitModel.AddComponent("Bs","Exponential",&NBs); (void)BkgMod;
//  BkgMod->FixShapeTo(Bdata);
//  Bdata->plotOn(Bframe,Binning(nbins));
//  BkgMod->GetPDF()->plotOn(Bframe);
//  if(drawpulls)
//  {
//    RooPlot* pullframe = x->frame(Title("Pull"));
//    pullframe->addPlotable(Bframe->pullHist(),"B");
//    plotter = new plotmaker<RooPlot>(Bframe);
//    plotter->SetPullPlot(pullframe);
//  }
//  else
//  {
//    plotter = new plotmaker<RooPlot>(Bframe);
//  }
//  plotter->SetTitle(xtitle, unit);
//  plotter->Draw()->SaveAs((plotname+"B.pdf").c_str());
/*Fit to collision data********************************************************/
  SigMod->FloatPar("mean");
  FitModel.Fit(Cdata);
  Cdata->plotOn(Cframe,Binning(nbins));
  FitModel.Plot(Cframe);
  Cframe->SetMaximum(Cframe->GetMaximum()*1.3);
  if(drawpulls)
  {
    RooPlot* pullframe = x->frame(Title("Pull"));
    pullframe->addPlotable(Cframe->pullHist(),"B");
    plotter = new plotmaker<RooPlot>(Cframe);
    plotter->SetPullPlot(pullframe);
  }
  else
  {
    plotter = new plotmaker<RooPlot>(Cframe);
  }
  plotter->SetBlurb(blurb);
  plotter->SetTitle(xtitle, unit);
  plotter->Draw()->SaveAs((plotname+".pdf").c_str());
/*S-weight the given ntuple****************************************************/
  if(doSweight)
  {
    using namespace RooStats;
    string trailer = "_Sweighted.root";
    string outputName = filename.substr(0, filename.size() - 5) + trailer;
    TFile* outputFile = TFile::Open(outputName.c_str(),"RECREATE");
    outputFile->cd();
    TTree* newtree = tree->CloneTree(0);
    cout << "copied the tree" << endl;
    RooStats::SPlot* sData = FitModel.GetsPlot(RooArgList(NLb,NBs));
    sData->GetName(); // Just to prevent compiler warning
    float Nsig_sw; newtree->Branch("Nsig_sw", &Nsig_sw,"Nsig_sw/F");
    float Nbkg_sw; newtree->Branch("Nbkg_sw", &Nbkg_sw,"Nbkg_sw/F");
    float L_Nsig;  newtree->Branch("L_Nsig",  &L_Nsig, "L_Nsig/F" );
    float L_Nbkg;  newtree->Branch("L_Nbkg",  &L_Nbkg, "L_Nbkg/F" );
    for (int i = 0; i < Cdata->numEntries(); i++)
    {
      tree->GetEntry(i);
      const RooArgSet* row = Cdata->get(i);
      Nsig_sw =  row->getRealValue("LbN_sw");
      L_Nsig  =  row->getRealValue("L_LbN" );
      Nbkg_sw =  row->getRealValue("BsN_sw");
      L_Nbkg  =  row->getRealValue("L_BsN" );
      newtree->Fill();
      if((i%1000)==0) cout << i << "/" << Cdata->numEntries() << endl;
    }
    cout << "Done" << endl;
    newtree->Write();
    outputFile->Close();
    cout << "Written to " << outputFile->GetName() << endl;
  }
  vector<parameter> pars;
  pars.push_back(parameter("N","N_{\\Lambda_{b}}",SigMod));
  pars.push_back(parameter("N","N_{B^0_{s}}",BkgMod));
/******************************************************************************/
  if(resname!="")
  {
    ResultDB table(DBfilename);
    for(auto& par : pars)
      table.Update(resname+par.safename(),"N",par.value,par.error);
    table.Save();
    cout << "Results saved to " << DBfilename << endl;
  }
  for(auto& par : pars)
    cout << "$" << par.latex << "$ & $" << par.value << " \\pm " << par.error << "$ \\\\" << endl;
}
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  std::string file, sfile, bfile, branch, model, cuts, xtitle, unit, plot, blurb, dbf, resname;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                             "produce help message"  )
    ("sweight" ,                                                                             "apply sweights to data")
    ("pulls,P" ,                                                                             "plot with pulls"       )
    ("blurb"   , value<string>(&blurb )                                                    , "blurb text"            )
    ("file,F"  , value<string>(&file  )->default_value("ntuples/BsphiKK_data_mvaVars.root"), "data file"             )
    ("Sfile"   , value<string>(&sfile )->default_value("ntuples/LbphiKp_MC_mvaVars.root"  ), "signal MC file"        )
    ("Bfile"   , value<string>(&bfile )->default_value("ntuples/BsphiKK_MC_mvaVars.root"  ), "background MC file"    )
    ("branch,B", value<string>(&branch)->default_value("phiKpM"                           ), "branch to plot"        )
    ("model,M" , value<string>(&model )->default_value("Crystal Ball + 2 Gaussians"       ), "model to use in fit"   )
    ("cuts,C"  , value<string>(&cuts  )->default_value("KK_M<1800"                        ), "optional cuts"         )
    ("title,T" , value<string>(&xtitle)->default_value("#it{m}(#it{#phi Kp})"             ), "x-axis title"          )
    ("unit,U"  , value<string>(&unit  )->default_value("MeV/#it{c}^{2}"                   ), "unit"                  )
    ("plot,O"  , value<string>(&plot  )->default_value("plottedbranch"                    ), "output plot filename"  )
    ("upper,u" , value<double>(&xup   )->default_value(6000                               ), "branch upper limit"    )
    ("lower,l" , value<double>(&xlow  )->default_value(5500                               ), "branch lower limit"    )
    ("bins,b"  , value<int   >(&nbins )->default_value(50                                 ), "number of bins"        )
    ("output-file" , value<string>(&dbf       )->default_value("FitResults.csv"           ), "output file"           )
    ("save-results", value<string>(&resname   )                                           , "name to save results as")
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  cout << "Entering main function" << endl;
  FitLb(file,sfile,bfile,branch,model,xtitle,unit,plot,cuts,xlow,xup,nbins,vmap.count("pulls"),blurb,vmap.count("sweight"), resname, dbf);
  return 0;
}

