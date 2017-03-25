// Standard headers
#include <string>
#include <iostream>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
// Custom headers
#include "annotation.h"
#include "plotmaker.h"
#include "MakeBranchPlot.h"
using std::string;
using std::cout;
using std::endl;
void PlotBranch(string filename, string branchname, string xtitle, string unit, string plotname, string cuts, string weight, double xlow, double xup, int nbins, string overlayfilename, double scale, bool saveasrootfile, bool drawline, double lineat)
{
  TH1D* frame = MakeBranchPlot(filename, branchname, cuts, weight, xlow, xup, nbins);
  string histname = plotname.find_last_of("/") == string::npos? plotname : plotname.substr(plotname.find_last_of("/")+1);
  frame->SetName(histname.c_str());
  frame->SetMaximum(frame->GetMaximum()*1.3);
  frame->SetMinimum(0);
  plotmaker plotter(frame);
  plotter.SetTitle(xtitle, unit);
  TCanvas* plot = plotter.Draw("E1");
  if(overlayfilename!="")
  {
    TH1D* overlay = MakeBranchPlot(overlayfilename, branchname, cuts, weight, xlow, xup, nbins);
    if(scale>0) overlay->Scale(scale);
    else overlay->Scale(frame->Integral()/overlay->Integral());
    overlay->SetDrawOption("B");
    overlay->SetFillColor(kOrange);
    overlay->SetLineColor(kOrange);
    overlay->Draw("same");
    frame->Draw("E1 same");
  }
  TLine* cutvalline;
  if(drawline)
  {
    cutvalline = new TLine(lineat,0,lineat,frame->GetMaximum());
    cutvalline->SetLineStyle(2);
    cutvalline->SetLineColor(2);
  }
  cutvalline->Draw();
  plot->SaveAs((plotname+".pdf").c_str());
  if(saveasrootfile)
  {
    TFile* file = TFile::Open((histname+".root").c_str(),"RECREATE");
    frame->Write();
    plot->Write();
    file->Close();
  }
  delete cutvalline;
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  string file, branch, cuts, xtitle, unit, plot, weight, overlay;
  double xlow, xup, scale, lineat;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                                  "produce help message"                )
    ("root"    ,                                                                                  "save plot as .root file"             )
    ("file,F"  , value<string>(&file   )->default_value("ntuples/BsphiKK_data_mva.root"        ), "data file"                           )
    ("branch,B", value<string>(&branch )->default_value("B_s0_LOKI_Mass"                       ), "branch to plot"                      )
    ("weight,W", value<string>(&weight )->default_value(""                                     ), "weighting variable"                  )
    ("cuts,C"  , value<string>(&cuts   )->default_value(""                                     ), "optional cuts"                       )
    ("title,T" , value<string>(&xtitle )->default_value("#it{m}(#it{#phi K^{#plus}K^{#minus}})"), "x-axis title"                        )
    ("unit,U"  , value<string>(&unit   )->default_value("MeV/#it{c}^{2}"                       ), "unit"                                )
    ("plot,O"  , value<string>(&plot   )->default_value("plottedbranch"                        ), "output plot filename"                )
    ("upper,u" , value<double>(&xup    )->default_value(5600                                   ), "branch upper limit"                  )
    ("lower,l" , value<double>(&xlow   )->default_value(5200                                   ), "branch lower limit"                  )
    ("bins,b"  , value<int   >(&nbins  )->default_value(50                                     ), "number of bins"                      )
    ("overlay" , value<string>(&overlay)->default_value(""                                     ), "file to overlay the same branch from")
    ("scale"   , value<double>(&scale  )->default_value(1                                      ), "scale factor for the overlaid branch")
    ("lineat"  , value<double>(&lineat )->default_value(0                                      ), "draw vertical line at this value"    )
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
  PlotBranch(file,branch,xtitle,unit,plot,cuts,weight,xlow,xup,nbins,overlay,scale,vmap.count("root"),vmap.count("lineat"),lineat);
  return 0;
}
