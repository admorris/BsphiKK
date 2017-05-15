// Standard headers
#include <string>
#include <iostream>
#include <vector>
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
#include "MassFitter.h"
#include "plotmaker.h"
#include "GetTree.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::sprintf;
void mKKfit(string filename, string branchname, string cuts, string weight, string xtitle, string unit, string plotname, double xlow, double xup, double yup, int nbins, bool convolve, bool fitacceptance, bool fitnonres, bool fitfzero, bool fitftwop, bool dontfitphi,std::string blurb)
{
/*Physics parameters**********************************************************/
  // Define in MeV
  double mBs = 5366.77; // TODO: turn this into a map (or one map per dimension: energy, inverse energy and unitless) and do the MeV to GeV conversion at the top
  double mfzero = 990;//939.9;
  double gpipi = 199;
  double gKK = gpipi*3;
  double mftwop = 1522.2;
  double Gftwop = 84;
  double mphi = 1019.461;
  double Gphi = 4.266;
  double mpi = 139.57018;
  double mK = 493.677;
  double BWBFradius = 0.0031;
  double mKKresolution_lo = 0.2;
  double mKKresolution_hi = 0.6;
  double mKKresolution_MC = 0.4;
/*Input***********************************************************************/
  TFile* file = TFile::Open(filename.c_str());
  TTree* tree = GetTree(file,cuts);
/*Fit the resolution**********************************************************/
  using namespace RooFit;
/*Do convolved fit************************************************************/
  double threshold = unit.find("GeV")!=string::npos ? 2e-3*mK : 2*mK;
  RooRealVar* m = new RooRealVar(branchname.c_str(),xtitle.c_str(),std::min(threshold,xlow),xup);
  cout << "Importing tree" << endl;
  RooDataSet* data;
  MassFitter* massfitter = new MassFitter(m);
  if(weight == "")
    data = new RooDataSet("data","",RooArgSet(*m),Import(*tree));
  else
  {
    RooRealVar* w = new RooRealVar(weight.c_str(),"weighting variable",-100,100);
    data = new RooDataSet("data","",RooArgSet(*m,*w),Import(*tree),WeightVar(*w));
    massfitter->SetWeighted();
  }
  RooRealVar* Nnonres  = new RooRealVar("N","Number of non-resonant candidates",200,0,1e6);
  RooRealVar* Nfzero   = new RooRealVar("N","Number of f0(980) candidates",150,0,1e6);
  RooRealVar* Nphi     = new RooRealVar("N","Number of phi candidates",3000,0,1e6);
  RooRealVar* Nftwop   = new RooRealVar("N","Number of f2'(1525) candidates",200,0,1e6);
  RooRealVar* thrscale = new RooRealVar("thrscale","Threshold shape scale",100.,0.,1e4);
  RooRealVar* accslope = new RooRealVar("accslope","Acceptance slope",0.,-100.,0.);
/*Construct acceptance function***********************************************/
  if(fitacceptance)
  {
    char function[100];
    sprintf(function,"TMath::Erf(@1*(@0-%f))*(1+@2*(@0-%f))",threshold,threshold);
//    sprintf(function,"TMath::TanH(@1*(@0-%f))*(1+@2*(@0-%f))",threshold,threshold);
//    sprintf(function,"TMath::ATan(@1*(@0-%f))*(1+@2*(@0-%f))*2.0*TMath::InvPi()",threshold,threshold);
    massfitter->SetWeightFunction(new RooFormulaVar("acceptance",function,RooArgList(*m,*thrscale,*accslope)));
  }
/*Non-resonant kaon pairs*****************************************************/
  Component* nonres;
  if(fitnonres)
  {
    nonres = massfitter->AddComponent("nonres","Three Body Phase Space",Nnonres);
    if(unit.find("GeV")!=string::npos)
    {
      nonres->SetRange("M",mBs*1e-3,mBs*1e-3);
      nonres->FixValue("M",mBs*1e-3);
      nonres->SetRange("m1",mK*1e-3,mK*1e-3);
      nonres->FixValue("m1",mK*1e-3);
      nonres->SetRange("m2",mK*1e-3,mK*1e-3);
      nonres->FixValue("m2",mK*1e-3);
      nonres->SetRange("m3",mphi*1e-3,mphi*1e-3);
      nonres->FixValue("m3",mphi*1e-3);
    }
    else
    {
      nonres->SetRange("M",mBs,mBs);
      nonres->FixValue("M",mBs);
      nonres->SetRange("m1",mK,mK);
      nonres->FixValue("m1",mK);
      nonres->SetRange("m2",mK,mK);
      nonres->FixValue("m2",mK);
      nonres->SetRange("m3",mphi,mphi);
      nonres->FixValue("m3",mphi);
    }
    nonres->SetColour(kCyan+1);
    nonres->SetStyle(kDotted);
  }
/*f0(980)*********************************************************************/
  Component* fzero;
  if(fitfzero)
  {
    fzero = massfitter->AddComponent("fzero","Flatte",Nfzero);
    fzero->SetColour(kGreen+1);
    fzero->SetStyle(9);
//    fzero->FixValue("spin",0);
    if(unit.find("GeV")!=string::npos)
    {
      fzero->SetRange("mean",mfzero*1e-3,mfzero*1e-3);
      fzero->FixValue("mean",mfzero*1e-3);
      fzero->SetRange("gpipi",gpipi*1e-3,gpipi*1e-3);
      fzero->FixValue("gpipi",gpipi*1e-3);
      fzero->SetRange("mpi",mpi*1e-3,mpi*1e-3);
      fzero->FixValue("mpi",mpi*1e-3);
      fzero->SetRange("gKK",gKK*1e-3,gKK*1e-3);
      fzero->FixValue("gKK",gKK*1e-3);
      fzero->SetRange("mK",mK*1e-3,mK*1e-3);
      fzero->FixValue("mK",mK*1e-3);
    }
    else
    {
      fzero->SetRange("mean",mfzero,mfzero);
      fzero->FixValue("mean",mfzero);
      fzero->SetRange("gpipi",gpipi,gpipi);
      fzero->FixValue("gpipi",gpipi);
      fzero->SetRange("mpi",mpi,mpi);
      fzero->FixValue("mpi",mpi);
      fzero->SetRange("gKK",gKK,gKK);
      fzero->FixValue("gKK",gKK);
      fzero->SetRange("mK",mK,mK);
      fzero->FixValue("mK",mK);
    }
  }
/*phi(1020)*******************************************************************/
  Component* phi;
  if(!dontfitphi)
  {
    if(convolve)
    {
      phi = massfitter->AddComponent("phi","RBW(X)Gauss",Nphi);
    }
    else
    {
      phi = massfitter->AddComponent("phi","Rel Breit-Wigner",Nphi);
    }
    phi->SetColour(kMagenta+1);
    phi->SetStyle(9);
    phi->FixValue("spin",1);
    if(unit.find("GeV")!=string::npos)
    {
      phi->SetRange("mean",mphi*1e-3,mphi*1e-3);
      phi->FixValue("mean",mphi*1e-3);
      phi->SetRange("width",Gphi*1e-3,Gphi*1e-3);
      phi->FixValue("width",Gphi*1e-3);
      phi->SetRange("radius",BWBFradius*1e3,BWBFradius*1e3);
      phi->FixValue("radius",BWBFradius*1e3);
      phi->SetRange("mK",mK*1e-3,mK*1e-3);
      phi->FixValue("mK",mK*1e-3);
      if(convolve)
      {
        phi->SetRange("sigma1",mKKresolution_lo*1e-3,mKKresolution_hi*1e-3);
        phi->FixValue("sigma1",mKKresolution_MC*1e-3);
      }
    }
    else
    {
      phi->SetRange("mean",mphi,mphi);
      phi->FixValue("mean",mphi);
      phi->SetRange("width",Gphi,Gphi);
      phi->FixValue("width",Gphi);
      phi->SetRange("radius",BWBFradius,BWBFradius);
      phi->FixValue("radius",BWBFradius);
      phi->SetRange("mK",mK,mK);
      phi->FixValue("mK",mK);
      if(convolve)
      {
        phi->SetRange("sigma1",mKKresolution_lo,mKKresolution_hi);
        phi->FixValue("sigma1",mKKresolution_MC);
      }
    }
  }
/*f2'(1525)*******************************************************************/
  Component* ftwop;
  if(fitftwop)
  {
    ftwop = massfitter->AddComponent("ftwop","Rel Breit-Wigner",Nftwop);
    ftwop->FixValue("spin",2);
    ftwop->SetColour(kOrange+2);
    ftwop->SetStyle(9);
    if(unit.find("GeV")!=string::npos)
    {
      ftwop->SetRange("mean",mftwop*1e-3,mftwop*1e-3);
      ftwop->FixValue("mean",mftwop*1e-3);
      ftwop->SetRange("width",Gftwop*1e-3,Gftwop*1e-3);
      ftwop->FixValue("width",Gftwop*1e-3);
      ftwop->SetRange("radius",BWBFradius*1e3,BWBFradius*1e3);
      ftwop->FixValue("radius",BWBFradius*1e3);
      ftwop->SetRange("mK",mK*1e-3,mK*1e-3);
      ftwop->FixValue("mK",mK*1e-3);
    }
    else
    {
      ftwop->SetRange("mean",mftwop,mftwop);
      ftwop->FixValue("mean",mftwop);
      ftwop->SetRange("width",Gftwop,Gftwop);
      ftwop->FixValue("width",Gftwop);
      ftwop->SetRange("radius",BWBFradius,BWBFradius);
      ftwop->FixValue("radius",BWBFradius);
      ftwop->SetRange("mK",mK,mK);
      ftwop->FixValue("mK",mK);
    }
  }
/*Do the fit******************************************************************/
  massfitter->Fit(data);
  RooPlot* frame = m->frame();
  cout << "Plotting" << endl;
  data->plotOn(frame,(Binning(nbins)));
  massfitter->Plot(frame);
  plotmaker fullplot(frame);
  fullplot.SetBlurb(blurb);
  RooHist* pullhist = frame->pullHist();
  RooPlot* pullframe = m->frame(Title("Pull"));
  pullframe->addPlotable(pullhist,"B");
  fullplot.SetPullPlot(pullframe);
  fullplot.SetTitle(xtitle, unit);
  if(yup>0)
    frame->SetMaximum(yup);
  frame->SetMinimum(0);
  TCanvas* can = fullplot.Draw();
  stringstream ytitle;
  ytitle << "#font[132]{}Candidates / (" << (xup-xlow)/nbins << " " << unit << ")";
  frame->GetYaxis()->SetTitle(ytitle.str().c_str());
  can->SaveAs((plotname+".pdf").c_str());
  double xsplit = 1060;
  double lowbinwidth = 1;
  if(unit.find("GeV")!=string::npos)
  {
    xsplit*=1e-3;
    lowbinwidth*=1e-3;
  }
  if((xup-xlow) > 2*(xsplit-xlow))
  {
    // Make low range bins a certain width
    int nbins_lo = (xsplit-xlow)/lowbinwidth;
    // Get the same bin width for the high range
    int nbins_hi = nbins * (xup-xsplit)/(xup-xlow);
    // Make the frames
    RooPlot* frame_lo = m->frame(Range(xlow,xsplit),Bins(nbins_lo));
    RooPlot* frame_hi = m->frame(Range(xsplit,xup),Bins(nbins_hi));
    // Plot the data
    data->plotOn(frame_lo);
    data->plotOn(frame_hi);
    // Plot the PDFs
    massfitter->Plot(frame_lo);
    massfitter->Plot(frame_hi);
    // Set the y-axis ranges
    frame_lo->SetMaximum(yup);
    frame_lo->SetMinimum(0);
    frame_hi->SetMinimum(0);
    // Make the pull plots
    RooHist* pullhist_lo = frame_lo->pullHist();
    RooHist* pullhist_hi = frame_hi->pullHist();
    RooPlot* pullframe_lo = m->frame(Title("Pull"),Range(xlow,xsplit));
    RooPlot* pullframe_hi = m->frame(Title("Pull"),Range(xsplit,xup));
    pullframe_lo->addPlotable(pullhist_lo,"B");
    pullframe_hi->addPlotable(pullhist_hi,"B");
    // Draw the low-mass plot
    plotmaker lowplot(frame_lo);
    lowplot.SetBlurb(blurb);
    lowplot.SetPullPlot(pullframe_lo);
    lowplot.SetTitle(xtitle, unit);
    can = lowplot.Draw();
    stringstream ytitle_lo;
    ytitle_lo << "#font[132]{}Candidates / (" << lowbinwidth << " " << unit << ")";
    frame_lo->GetYaxis()->SetTitle(ytitle_lo.str().c_str());
    can->SaveAs((plotname+"_low_range.pdf").c_str());
    // Draw the high-mass plot
    plotmaker highplot(frame_hi);
    highplot.SetBlurb(blurb);
    highplot.SetPullPlot(pullframe_hi);
    highplot.SetTitle(xtitle, unit);
    can = highplot.Draw();
    frame_hi->GetYaxis()->SetTitle(ytitle.str().c_str());
    can->SaveAs((plotname+"_high_range.pdf").c_str());
  }
}
/*Main function***************************************************************/
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  using std::string;
  options_description desc("Allowed options",120);
  string file, branch, cuts, xtitle, unit, plot, weight, blurb;
  double xlow, xup, yup;
  int nbins;
  desc.add_options()
    ("help,H"    ,                                                                                 "produce help message")
    ("blurb", value<std::string>(&blurb), "blurb text")
    ("conv"      ,                                                                                 "do convolution"      )
    ("fitacc"    ,                                                                                 "fit for acceptance"  )
    ("fitnonres" ,                                                                                 "use nonres component")
    ("fitfzero"  ,                                                                                 "use f0 component"    )
    ("fitftwop"  ,                                                                                 "use f2' component"   )
    ("dontfitphi",                                                                                 "no phi component"    )
    ("file,F"    , value<string>(&file  )->default_value("ntuples/BsphiKK_data_1800_mvacut.root"), "data ntuple"         )
    ("branch,B"  , value<string>(&branch)->default_value("KK_M"                                 ), "mass branch"         )
    ("cuts,C"    , value<string>(&cuts  )->default_value(""                                     ), "cuts"                )
    ("weight,W"  , value<string>(&weight)->default_value(""                                     ), "weight"              )
    ("title,T"   , value<string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}})"     ), "x-axis title"        )
    ("unit,U"    , value<string>(&unit  )->default_value("MeV/#it{c}^{2}"                       ), "x-axis unit"         )
    ("plot,O"    , value<string>(&plot  )->default_value("KKmassfit"                            ), "output plot filename")
    ("upper,u"   , value<double>(&xup   )->default_value(1800                                   ), "x-axis upper limit"  )
    ("lower,l"   , value<double>(&xlow  )->default_value(980                                    ), "x-axis lower limit"  )
    ("ymax"      , value<double>(&yup   )->default_value(-1                                     ), "y-axis upper limit"  )
    ("bins,b"    , value<int   >(&nbins )->default_value(41                                     ), "number of bins"      )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    cout << desc << endl;
    return 1;
  }
  cout << "Entering main function" << endl;
  mKKfit(file,branch,cuts,weight,xtitle,unit,plot,xlow,xup,yup,nbins,vmap.count("conv"),vmap.count("fitacc"),vmap.count("fitnonres"),vmap.count("fitfzero"),vmap.count("fitftwop"),vmap.count("dontfitphi"),blurb);
  return 0;
}

