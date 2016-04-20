// Standard headers
#include <iostream>
#include <string>
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// RooFit headers
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooHist.h"
#include "RooRealVar.h"
// Custom headers
#include "GetTree.h"
#include "MakeBranchPlot.h"
using std::string;
using std::cout;
using std::endl;
using namespace RooFit;
RooPlot* MakeBranchPlot(string filename, string branchname, string xtitle, string unit, string plotname, string cuts, string weight, double xlow, double xup, int nbins, string overlay, double scale)
{

  TFile* file = new TFile(filename.c_str()), * ofile;
  TTree* tree = GetTree(file,cuts), * otree;
  RooRealVar* x = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
  x->setBins(nbins);
  RooRealVar* w;
  RooDataSet* data, * odata;
  RooDataHist* overlayhist;
  bool plotoverlay = overlay != "";
  if(plotoverlay)
  {
    ofile = new TFile(overlay.c_str());
    otree = GetTree(ofile,cuts);
    file->cd();
    cout << "Importing tree" << endl;
    odata = new RooDataSet("overlaydata","",RooArgSet(*x),Import(*otree));
    overlayhist = new RooDataHist("overlayhist","",RooArgSet(*x),*odata,scale);
  }
  cout << "Importing tree" << endl;
  if(weight!="")
  {
    w = new RooRealVar(weight.c_str(),"",-0.5,1.5);
    data = new RooDataSet("data","",RooArgSet(*x,*w),WeightVar(*w),Import(*tree));
  }
  else
  {
    data = new RooDataSet("data","",RooArgSet(*x),Import(*tree));
  }
  RooPlot* frame = x->frame();
  cout << "Plotting" << endl;
  if(plotoverlay) overlayhist->plotOn(frame,Binning(nbins),DrawOption("B1"),FillColor(kOrange),LineWidth(0));
  data->plotOn(frame,Binning(nbins));
  frame->SetMinimum(0);
  return frame;
}
