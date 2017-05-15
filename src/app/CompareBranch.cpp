// Standard headers
#include <string>
#include <iostream>
#include <algorithm>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TMath.h"
#include "TLine.h"
// Custom headers
#include "MakeBranchPlot.h"
#include "plotmaker.h"
void CompareBranch(std::string MCfilename, std::string CDfilename, std::string MCbranchname, std::string CDbranchname, std::string xtitle, std::string unit, std::string plotname, std::string MCcuts, std::string CDcuts, std::string MCweight, std::string CDweight, double xlow, double xup, int nbins, bool drawline, double lineat,std::string blurb)
{
  TH1D*  MChist = MakeBranchPlot(MCfilename,MCbranchname,MCcuts,MCweight,xlow,xup,nbins);
  TH1D*  CDhist = MakeBranchPlot(CDfilename,CDbranchname,CDcuts,CDweight,xlow,xup,nbins);
  MChist->Scale(1.0/MChist->Integral());
  CDhist->Scale(1.0/CDhist->Integral());
  MChist->SetFillColor(kOrange);
  MChist->SetLineColor(kOrange);
  MChist->SetMaximum(std::max(CDhist->GetMaximum(),MChist->GetMaximum())*1.3);
  MChist->SetMinimum(0);
  // Draw everything
  plotmaker<TH1> plotter(MChist);
  plotter.SetBlurb(blurb);
  plotter.SetTitle(xtitle, unit);
  TCanvas* plot = plotter.Draw("HIST");
  CDhist->Draw("sameE1");
  TLine* cutvalline;
  if(drawline)
  {
    cutvalline = new TLine(lineat,0,lineat,MChist->GetMaximum());
    cutvalline->SetLineStyle(2);
    cutvalline->SetLineColor(2);
  }
  cutvalline->Draw();
  plot->SaveAs((plotname+".pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",(unsigned)120);
  std::string MCfile, CDfile, MCbranch, CDbranch, MCcuts, CDcuts, xtitle, unit, plot, MCweight, CDweight, blurb;
  double xlow, xup, lineat;
  int nbins;
  desc.add_options()
    ("help"    ,                                                                                "produce help message"             )
    ("blurb", value<std::string>(&blurb), "blurb text")
    ("MCfile"  , value<std::string>(&MCfile  )->default_value("ntuples/BsphiKK_MC_mva.root"  ), "Monte Carlo file"                 )
    ("CDfile"  , value<std::string>(&CDfile  )->default_value("ntuples/BsphiKK_data_mva.root"), "collision data file"              )
    ("MCbranch", value<std::string>(&MCbranch)->default_value("B_s0_LOKI_Mass"               ), "Monte Carlo branch to plot"       )
    ("CDbranch", value<std::string>(&CDbranch)->default_value("B_s0_LOKI_Mass"               ), "collision data branch to plot"    )
    ("MCweight", value<std::string>(&MCweight)->default_value(""                             ), "Monte Carlo weighting variable"   )
    ("CDweight", value<std::string>(&CDweight)->default_value(""                             ), "collision data weighting variable")
    ("MCcuts"  , value<std::string>(&MCcuts  )->default_value(""                             ), "Monte Carlo cuts"                 )
    ("CDcuts"  , value<std::string>(&CDcuts  )->default_value(""                             ), "collision data cuts"              )
    ("title"   , value<std::string>(&xtitle  )->default_value("#it{m}(#it{#phi K^{+}K^{-}})" ), "x-axis title"                     )
    ("unit"    , value<std::string>(&unit    )->default_value("MeV/#it{c}^{2}"               ), "unit"                             )
    ("plot"    , value<std::string>(&plot    )->default_value("comparison"                   ), "output plot filename"             )
    ("upper"   , value<double     >(&xup     )->default_value(5600                           ), "branch upper limit"               )
    ("lower"   , value<double     >(&xlow    )->default_value(5200                           ), "branch lower limit"               )
    ("bins"    , value<int        >(&nbins   )->default_value(20                             ), "number of bins"                   )
    ("lineat"  , value<double     >(&lineat  )                                                , "draw vertical line at this value" )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }
  std::cout << "Entering main function" << std::endl;
  CompareBranch(MCfile,CDfile,MCbranch,CDbranch,xtitle,unit,plot,MCcuts,CDcuts,MCweight,CDweight,xlow,xup,nbins,vmap.count("lineat"),lineat,blurb);
  return 0;
}
