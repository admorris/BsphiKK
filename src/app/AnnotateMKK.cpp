// Standard headers
#include <string>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TLine.h"
#include "TLatex.h"
// RooFit headers
#include "RooDataSet.h"
#include "RooHist.h"
#include "RooPlot.h"
#include "RooRealVar.h"
// Custom headers
#include "plotmaker.h"

typedef struct
{
  TLine line;
  TLatex label;
} annotation;

annotation* resonance(double mass,string name)
{
  annotation* _a = new annotation;
  _a->line.SetY1(0);
  _a->line.SetY2(1);
  _a->line.SetX1(mass);
  _a->line.SetX2(mass);
  _a->line.SetLineStyle(2);
  _a->line.SetLineColor(2);
  _a->label.SetText(mass+50,1.0,name.c_str());
  return _a;
}

void PlotBranch(string filename, string branchname, string plotname, string weight, double xlow, double xup, int nbins)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  using namespace RooFit;
  RooRealVar* x = new RooRealVar(branchname.c_str(),"#it{m}(#it{K^{#plus}K^{#minus}})",xlow,xup);
  RooRealVar* w;
  RooDataSet* data;
  std::cout << "Importing tree" << endl;
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
  std::cout << "Plotting" << endl;
  data->plotOn(frame,Binning(nbins));
  RooHist* hist = frame->getHist();
  annotation* particles[] = 
  {
    resonance(1019.461,"#it{#phi}")
  , resonance(1864.84,"#it{D}^{0}")
  , resonance(1525,"#it{f'}_{2}(1525)")
  , resonance(3096.916,"#it{J/#psi}")
  , resonance(3414.75,"#it{#chi}_{#it{c}0}")
  };
  plotmaker plotter(frame);
  plotter.SetTitle("#it{m}(#it{K^{#plus}K^{#minus}})", "MeV/#it{c}^{2}");
  TCanvas* canv = plotter.Draw();
  TLatex* label;
  TLine* line;
  double height;
  for(unsigned int i = 0; i < sizeof(particles)/sizeof(annotation*); i++)
  {
    line = &(particles[i]->line);
    label = &(particles[i]->label);
    height = hist->Eval(line->GetX1());
    if(line->GetX1()>xlow && line->GetX1()<xup && height/frame->GetMaximum() > 0.05)
    {
      line->SetY2(line->GetY2()*height);
      line->Draw();
      label->SetY(label->GetY()*height+0.075*frame->GetMaximum());
      label->Draw();
    }
    else
    {
      cout << "Skipping particle at " << line->GetX1() << " MeV" << endl;
    }
  }
  canv->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string file, branch, plot, weight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                                      "produce help message"                      )
    ("file,F"  , value<std::string>(&file  )->default_value("ntuples/BsphiKK_data_mva.root"        ), "set data file"                             )
    ("branch,B", value<std::string>(&branch)->default_value("KK_M"                                 ), "set branch to plot"                        )
    ("weight,W", value<std::string>(&weight)->default_value("Nsig_sw"                              ), "set weighting variable"                    )
    ("plot,O"  , value<std::string>(&plot  )->default_value("plottedbranch"                        ), "set output plot filename"                  )
    ("upper,u" , value<double     >(&xup   )->default_value(4000                                   ), "set branch upper limit"                    )
    ("lower,l" , value<double     >(&xlow  )->default_value(900                                    ), "set branch lower limit"                    )
    ("bins,b"  , value<int        >(&nbins )->default_value(50                                     ), "set number of bins"                        )
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
  PlotBranch(file,branch,plot,weight,xlow,xup,nbins);
  return 0;
}
