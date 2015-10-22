// Standard headers
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TMath.h"
// RooFit headers
#include "RooDataSet.h"
#include "RooHist.h"
#include "RooPlot.h"
#include "RooRealVar.h"
// Custom headers
#include "plotmaker.h"
// Just sum bin contents for a RooHist 
inline double integrate(RooHist* hist)
{
  double integral = 0;
  for(int i = 0; i < hist->GetN(); i++)
  {
    integral+=hist->GetY()[i];
  }
  return integral;
}
void PlotBranch(string MCfilename, string REfilename, string branchname, string xtitle, string unit, string plotname, string cuts, string MCweight, string REweight, double xlow, double xup, int nbins)
{
  // Open the files and get the trees
  TFile* MCfile = new TFile(MCfilename.c_str());
  TFile* REfile = new TFile(REfilename.c_str());
  TTree* MCtree = (TTree*)MCfile->Get("DecayTree");
  TTree* REtree = (TTree*)REfile->Get("DecayTree");
  // RooFit variables
  using namespace RooFit;
  RooRealVar* x = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
  RooRealVar* MCw,* REw;
  RooDataSet* MCdata,* REdata;
  // Store the values of each bin because the GetMaximum() method isn't implemented properly in RooHist
  vector<double> bincontent;
  // Get the data out of the file and optionally weight it
  std::cout << "Importing first tree" << endl;
  if(MCweight!="")
  {
    MCw = new RooRealVar(MCweight.c_str(),"",-0.5,1.5);
    MCdata = new RooDataSet("MCdata","",RooArgSet(*x,*MCw),WeightVar(*MCw),Import(*MCtree));
  }
  else
  {
    MCdata = new RooDataSet("MCdata","",RooArgSet(*x),Import(*MCtree));
  }
  std::cout << "Importing second tree" << endl;
  if(REweight!="")
  {
    REw = new RooRealVar(REweight.c_str(),"",-0.5,1.5);
    REdata = new RooDataSet("REdata","",RooArgSet(*x,*REw),WeightVar(*REw),Import(*REtree));
  }
  else
  {
    REdata = new RooDataSet("REdata","",RooArgSet(*x),Import(*REtree));
  }
  // Create a RooPlot and add the data points
  RooPlot* frame = x->frame();
  std::cout << "Plotting" << endl;
  MCdata->plotOn(frame,Binning(nbins),DrawOption("B1"),FillColor(kOrange));
  REdata->plotOn(frame,Binning(nbins));
  // Get the histograms out of the RooPlot
  RooHist* h_MCdata = frame->getHist("h_MCdata");
  RooHist* h_REdata = frame->getHist("h_REdata");
  // Get rid of MC datapoint error bars
  for(int i = 0; i < h_MCdata->GetN(); i++)
  {
    h_MCdata->SetPointError(i,0,0,0,0);
    bincontent.push_back(h_MCdata->GetY()[i]);
  }
  // Integrate the histograms
  double MCint = integrate(h_MCdata);
  double REint = integrate(h_REdata);
  double ratio = MCint/REint;
  cout << "First integral \t" << MCint << endl;
  cout << "Second integral\t" << REint << endl;
  cout << "Scale factor   \t" << ratio << endl;
  // Scale the data plot to match the Monte Carlo
  for(int i = 0; i < h_REdata->GetN(); i++)
  {
    h_REdata->GetY(     )[i] *= ratio;
    h_REdata->GetEYhigh()[i] *= ratio;
    h_REdata->GetEYlow( )[i] *= ratio;
    bincontent.push_back(h_REdata->GetY()[i]);
  }
  // Set a sensible maximum so the blurb doesn't sit on top of data points
  double max = *max_element(bincontent.begin(),bincontent.end());
  cout << "The maximum is\t" << max << endl;
  frame->SetMaximum(max*1.3);
  // Draw everything
  plotmaker plotter(frame);
  plotter.SetTitle(xtitle, unit);
  plotter.Draw()->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",(unsigned)120);
  std::string MCfile, REfile, branch, cuts, xtitle, unit, plot, MCweight, REweight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"    ,                                                                                        "produce help message"                      )
    ("MCfile,M"  , value<std::string>(&MCfile  )->default_value("ntuples/BsphiKK_MC_mva.root"          ), "set Monte Carlo file"                      )
    ("REfile,R"  , value<std::string>(&REfile  )->default_value("ntuples/BsphiKK_data_mva.root"        ), "set collision data file"                   )
    ("branch,B"  , value<std::string>(&branch  )->default_value("B_s0_LOKI_Mass"                       ), "set branch to plot"                        )
    ("MCweight,w", value<std::string>(&MCweight)->default_value(""                                     ), "set Monte Carlo weighting variable"        )
    ("REweight,W", value<std::string>(&REweight)->default_value(""                                     ), "set collision data weighting variable"     )
    ("cuts,C"    , value<std::string>(&cuts    )->default_value(""                                     ), "set optional cuts (UNIMPLEMENTED)"         )
    ("title,T"   , value<std::string>(&xtitle  )->default_value("#it{m}(#it{#phi K^{#plus}K^{#minus}})"), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"    , value<std::string>(&unit    )->default_value("MeV/#it{c}^{2}"                       ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"    , value<std::string>(&plot    )->default_value("comparison"                           ), "set output plot filename"                  )
    ("upper,u"   , value<double     >(&xup     )->default_value(5600                                   ), "set branch upper limit"                    )
    ("lower,l"   , value<double     >(&xlow    )->default_value(5200                                   ), "set branch lower limit"                    )
    ("bins,b"    , value<int        >(&nbins   )->default_value(20                                     ), "set number of bins"                        )
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
  PlotBranch(MCfile,REfile,branch,xtitle,unit,plot,cuts,MCweight,REweight,xlow,xup,nbins);
  return 0;
}
