#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TChain.h"
#include "TH1F.h"
#include <iostream>
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooExponential.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooUniform.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooStats/SPlot.h"
#include "TCut.h"
#include <fstream>
#include <string>
/******************************************************************************/
using namespace RooFit ;
void sweightedmassfit(string input = "BsphiKK_data_mvacut.root")
{
/*Open files and get trees*****************************************************/
  string folder = "../ntuples/";
  TFile* BsphiKKdatafile = new TFile((folder+input                  ).c_str()),
       * BsphiKKMCfile   = new TFile((folder+"BsphiKK_MC_mvacut.root"  ).c_str()),
       * BdphiKstMCfile  = new TFile((folder+"BdphiKst_MC_mvacut.root" ).c_str()),
       * LbphiKpMCfile   = new TFile((folder+"LbphiKp_MC_mvacut.root"  ).c_str());
  TTree* BsphiKKdatatree = (TTree*)BsphiKKdatafile->Get("DecayTree"),
       * BsphiKKMCtree   = (TTree*)BsphiKKMCfile->Get  ("DecayTree"),
       * BdphiKstMCtree  = (TTree*)BdphiKstMCfile->Get ("DecayTree"),
       * LbphiKpMCtree   = (TTree*)LbphiKpMCfile->Get  ("DecayTree");
/*Extract mass branches********************************************************/
  RooRealVar x("B_s0_LOKI_Mass","#font[12]{m_{K^{#plus}K^{#minus}K^{#plus}K^{#minus}}} #font[132]{[MeV/}#font[12]{c}#font[132]{^{2}}]",5250,5500);
  RooDataSet BsphiKKdata   ("BsphiKKdata"   ,"",RooArgSet(x),RooFit::Import(*BsphiKKdatatree)),
             BsphiKKMCdata ("BsphiKKMCdata" ,"",RooArgSet(x),RooFit::Import(*BsphiKKMCtree  )),
             BdphiKstMCdata("BdphiKstMCdata","",RooArgSet(x),RooFit::Import(*BdphiKstMCtree )),
             LbphiKpMCdata ("LbphiKpMCdata" ,"",RooArgSet(x),RooFit::Import(*LbphiKpMCtree  ));
  RooPlot* frame[4] = {x.frame(Title("BsphiKK Data")),
                       x.frame(Title("BsphiKK MC"  )),
                       x.frame(Title("BsphiKst MC" )),
                       x.frame(Title("LbphiKp MC"  ))};

  // Signal PDF to fit to BsphiKK MC
  RooRealVar    mean  ("mean"  ,"",5.36815e+03,5360,5380);
  RooRealVar    NsigMC("NsigMC","",35000,0,120000);
  RooRealVar    NbkgMC("NbkgMC","",250,0,20000);
  RooRealVar    sigma1("sigma1","",13,10,18);
  RooRealVar    sigma2("sigma2","",33,18,50);
  RooRealVar    sigma3("sigma3","",50,40,120);
  RooRealVar    fgaus1("fgaus1","",0.8,0.7,0.9);
  RooRealVar    fgaus2("fgaus2","",0.1,0.0,0.3);
  RooFormulaVar fgaus3("fgaus3","(1-@0-@1)*(@0+@1<1)",RooArgList(fgaus1,fgaus2));
  RooGaussian   gauss1("gauss1","First gaussian",x,mean,sigma1);
  RooGaussian   gauss2("gauss2","Second gaussian",x,mean,sigma2);
  RooGaussian   gauss3("gauss3","Third gaussian",x,mean,sigma3);
  RooUniform    MCbkgmod("MCbkgmod","Background pdf",x);
  RooAddPdf     tripleGau("tripleGau","tripleGau",RooArgList(gauss1,gauss2,gauss3),RooArgList(fgaus1,fgaus2,fgaus3));
  RooAddPdf     sigmod("sigmod","sigmod",RooArgList(tripleGau,MCbkgmod),RooArgList(NsigMC,NbkgMC));
  
  // Do fit to BsphiKK MC
  sigmod.fitTo(BsphiKKMCdata);
  BsphiKKMCdata.plotOn(frame[1]);
  sigmod.plotOn(frame[1]);
    
  // Signal PDF to fit to data
  RooRealVar    scale   ("scale","scale",1.0,0.0,2.0);
  RooConstVar   c_sigma1("c_sigma1","@0",sigma1.getValV()),
                c_sigma2("c_sigma2","@0",sigma2.getValV()),
                c_sigma3("c_sigma3","@0",sigma3.getValV()),
                c_ngaus1("c_ngaus1","@0",fgaus1.getValV()),
                c_ngaus2("c_ngaus2","@0",fgaus2.getValV()),
                c_ngaus3("c_ngaus3","@0",fgaus3.getValV());
  RooFormulaVar f_sigma1("f_sigma1","@0*@1",RooArgList(c_sigma1,scale)),
                f_sigma2("f_sigma2","@0*@1",RooArgList(c_sigma2,scale)),
                f_sigma3("f_sigma3","@0*@1",RooArgList(c_sigma3,scale));
  RooGaussian   c_gauss1("c_gauss1","",x,mean,f_sigma1);
  RooGaussian   c_gauss2("c_gauss2","",x,mean,f_sigma2);
  RooGaussian   c_gauss3("c_gauss3","",x,mean,f_sigma3);
  RooAddPdf     c_sigmod("c_sigmod","c_sigmod",RooArgList(c_gauss1,c_gauss2,c_gauss3),RooArgList(c_ngaus1,c_ngaus2,c_ngaus3));
  
  
  


  // Background PDF
  RooRealVar c("c","",0,-0.01,0.01);
  RooExponential bkgmod("bkgmod","",x,c);
  
  // Peaking backgrounds
	// lambda_b PDF
	RooConstVar c_lb_m0("c_lb_m0","c_lb_m0", 5467),
					    c_lb_sigma("c_lb_sigma","c_lb_sigma", 27),
					    c_lb_alpha("c_lb_alpha","c_lb_alpha", 0.62),
					    c_lb_n("c_lb_n","c_lb_n", 0.7);
	RooCBShape  c_lb_model("c_lb_model","",x,c_lb_m0,c_lb_sigma,c_lb_alpha,c_lb_n);
	
	// phi kstar PDF
	RooConstVar c_pk_m0("c_pk_m0","c_pk_m0", 5336),
					    c_pk_sigma("c_pk_sigma","c_pk_sigma", 21),
					    c_pk_alpha("c_pk_alpha","c_pk_alpha", -0.54),
					    c_pk_n("c_pk_n","c_pk_n", 17.6),
					    c_pk_f("c_pk_f","c_pk_f", 0.8788);
	RooCBShape  c_Bpk_CB("c_Bpk_CB","",x,c_pk_m0,c_pk_sigma,c_pk_alpha,c_pk_n);
	RooUniform  c_Bpk_Fl("c_Bpk_Fl","",x);
	RooAddPdf   c_Bpk_model("c_Bpk_model","",RooArgList(c_Bpk_CB,c_Bpk_Fl),c_pk_f);
  // Total PDF
  RooRealVar Nsig("Nsig","Number of signal events",7000,1,12000);
  RooRealVar Nbkg("Nbkg","Number of background events",5000,1,12000);
  RooFormulaVar Nlb("Nlb","@0*101/3947",RooArgList(Nsig)),
	              NBpk("NBpk","@0*25/3947",RooArgList(Nsig));
  RooAddPdf model("model","model",RooArgList(c_sigmod,bkgmod),RooArgList(Nsig,Nbkg));
  // Do fit to data
  model.fitTo(BsphiKKdata);
  
  BsphiKKdata.plotOn(frame[0]);
  model.plotOn(frame[0]);
  model.plotOn(frame[0],Components("c_sigmod"));
  model.plotOn(frame[0],Components("bkgmod"));
  frame[0]->Draw();
  
//  string outputName = ;
  string trailer = "_Sweighted.root";
  string outputName = folder + input.substr(0, input.size() - 5) + trailer;
  TFile* f_out = new TFile(outputName.c_str(),"RECREATE");
  TTree* newtree = BsphiKKdatatree->CloneTree(-1);
  cout << "copied the tree" << endl;
  RooStats::SPlot* sData = new RooStats::SPlot("sData","An SPlot", BsphiKKdata, &model, RooArgList(Nsig,Nbkg));
  float Nsig_sw; TBranch*  b_Nsig_sw = newtree->Branch("Nsig_sw", &Nsig_sw,"Nsig_sw/F");  
  float Nbkg_sw; TBranch*  b_Nbkg_sw = newtree->Branch("Nbkg_sw", &Nbkg_sw,"Nbkg_sw/F");
  float L_Nsig;  TBranch*  b_L_Nsig  = newtree->Branch("L_Nsig",  &L_Nsig, "L_Nsig/F" );
  float L_Nbkg;  TBranch*  b_L_Nbkg  = newtree->Branch("L_Nbkg",  &L_Nbkg, "L_Nbkg/F" );
  for (int i = 0; i < BsphiKKdata.numEntries(); ++i)
  {
    newtree->GetEntry(i);
    const RooArgSet* row = BsphiKKdata.get(i); 
    Nsig_sw =  row->getRealValue("Nsig_sw");
    L_Nsig  =  row->getRealValue("L_Nsig" );
    Nbkg_sw =  row->getRealValue("Nbkg_sw");
    L_Nbkg  =  row->getRealValue("L_Nbkg" ); 
    if (i < 10)
    cout << Nsig_sw << endl;
    b_Nsig_sw->Fill();
    b_L_Nsig->Fill();
    b_Nbkg_sw->Fill();
    b_L_Nbkg->Fill();
  }
  newtree->Write();
  f_out->Close();  

}
