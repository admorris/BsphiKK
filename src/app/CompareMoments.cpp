// Standard C++
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
// ROOT
#include "TLine.h"
// BOOST
#include "boost/program_options.hpp"
// Custom
#include "GetTree.h"
#include "plotmaker.h"
#include "LegendreMomentPlot.h"

void PlotMoments(std::string MCfilename, std::string CDfilename, std::string MCmassname, std::string CDmassname, std::string MCanglename, std::string CDanglename, std::string xtitle, std::string unit, std::string plotname, std::string MCcuts, std::string CDcuts, std::string MCweight, std::string CDweight, double xlow, double xup, int nbins, int max_order,std::string blurb)
{
  TCanvas allplots("allplots","",1200,1200);
  auto CDtree = std::unique_ptr<TTree>(GetTree(CDfilename,CDcuts));
  auto MCtree = std::unique_ptr<TTree>(GetTree(MCfilename,MCcuts));
  std::string CDmassrange = CDmassname + ">" + std::to_string(xlow) + "&&" + CDmassname + "<" + std::to_string(xup);
  std::string MCmassrange = MCmassname + ">" + std::to_string(xlow) + "&&" + MCmassname + "<" + std::to_string(xup);
  double scale = CDtree->GetEntries(CDmassrange.c_str())/(double)MCtree->GetEntries(MCmassrange.c_str());
  std::cout << "Number of data events in " << CDmassrange << " : " << CDtree->GetEntries(CDmassrange.c_str()) << "\n";
  std::cout << "Number of toy events in " << MCmassrange << " : " << MCtree->GetEntries(MCmassrange.c_str()) << "\n";
  std::cout << "The normalisation is " << scale << "\n";
  std::vector<LegendreMomentPlot> CDplots = LegendreMomentPlot::FillPlotsFromTree("data",*CDtree,CDmassname,CDanglename,CDweight,xtitle,unit,plotname,xlow,xup,nbins,max_order, 1);
  std::vector<LegendreMomentPlot> MCplots = LegendreMomentPlot::FillPlotsFromTree("model",*MCtree,MCmassname,MCanglename,MCweight,xtitle,unit,plotname,xlow,xup,nbins,max_order,scale);
  assert(MCplots.size() == CDplots.size());
  int ny = std::round(std::ceil(std::sqrt(max_order)));
  int nx = std::round(std::ceil(double(max_order)/ny));
  allplots.Divide(nx,ny);
  for(int order = 0; order <= max_order; order++)
  {
    TH1D residualplot(CDplots[order].hist);
    residualplot.Add(&MCplots[order].hist,-1);
    residualplot.SetFillColor(kBlack);
    for(int ibin = 1; ibin < nbins+1; ibin++) // Turn residuals into pulls (I think)
      residualplot.SetBinContent(ibin,residualplot.GetBinContent(ibin)/CDplots[order].hist.GetBinError(ibin));
    plotmaker<TH1> plot(&CDplots[order].hist);
    plot.SetBlurb(blurb);
    plot.SetPullPlot(&residualplot);
    plot.SetTitle(xtitle,unit);
    TCanvas* canvas = plot.Draw("hist B");
    CDplots[order].hist.SetDrawOption("E1");
    CDplots[order].hist.SetMarkerColor(kBlack);
    CDplots[order].hist.SetLineColor(kBlack);
    MCplots[order].hist.SetLineColor(kBlue);
    MCplots[order].hist.SetLineWidth(2);
    MCplots[order].hist.Draw("hist same");
    std::stringstream ytitle;
    ytitle << CDplots[order].hist.GetName() << " / " << (xup-xlow)/nbins << " " << unit;
    CDplots[order].hist.GetYaxis()->SetTitle(ytitle.str().c_str());
    canvas->SaveAs((plotname+"_P"+std::to_string(order)+".pdf").c_str());
    allplots.cd(order+1);
    CDplots[order].hist.Draw("E1");
    MCplots[order].hist.Draw("hist same");
  }
  allplots.SaveAs((plotname+"_all.pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string MCfile, CDfile, MCmass, CDmass, MCangle, CDangle, MCcuts, CDcuts, xtitle, unit, plot, MCweight, CDweight, blurb;
  double xlow, xup;
  int nbins, N;
  desc.add_options()
    ("help"    ,                                                                                "produce help message"             )
    ("blurb", value<std::string>(&blurb), "blurb text")
    ("MCfile"  , value<std::string>(&MCfile  )->default_value("toys.root"                    ), "Monte Carlo file"                 )
    ("CDfile"  , value<std::string>(&CDfile  )->default_value("ntuples/BsphiKK_data_mva.root"), "collision data file"              )
    ("MCmass"  , value<std::string>(&MCmass  )->default_value("mKK"                          ), "Monte Carlo mass branch"          )
    ("CDmass"  , value<std::string>(&CDmass  )->default_value("BCON_KK_M_GeV"                ), "collision data mass branch"       )
    ("MCangle" , value<std::string>(&MCangle )->default_value("ctheta_2"                     ), "Monte Carlo cosθ branch"          )
    ("CDangle" , value<std::string>(&CDangle )->default_value("cos_theta2"                   ), "collision data cosθ branch"       )
    ("MCweight", value<std::string>(&MCweight)->default_value(""                             ), "Monte Carlo weighting variable"   )
    ("CDweight", value<std::string>(&CDweight)->default_value(""                             ), "collision data weighting variable")
    ("MCcuts"  , value<std::string>(&MCcuts  )->default_value(""                             ), "Monte Carlo cuts"                 )
    ("CDcuts"  , value<std::string>(&CDcuts  )->default_value(""                             ), "collision data cuts"              )
    ("title"   , value<std::string>(&xtitle  )->default_value("#it{m}(#it{K^{+}K^{#minus}})" ), "x-axis title"                     )
    ("unit"    , value<std::string>(&unit    )->default_value("GeV/#it{c}^{2}"               ), "unit"                             )
    ("plot"    , value<std::string>(&plot    )->default_value("LegendreMoments"              ), "output plot filename"             )
    ("upper"   , value<double     >(&xup     )->default_value(1.80                           ), "x-axis upper limit"               )
    ("lower"   , value<double     >(&xlow    )->default_value(1.05                           ), "x-axis lower limit"               )
    ("bins"    , value<int        >(&nbins   )->default_value(25                             ), "number of bins"                   )
    ("max-order",value<int        >(&N       )->default_value(5                              ), "max polynomial order"             )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help") || N < 0 || nbins < 0 || xlow >= xup)
  {
    std::cout << desc << std::endl;
    return 1;
  }
  
  PlotMoments(MCfile, CDfile, MCmass, CDmass, MCangle, CDangle, xtitle, unit, plot, MCcuts, CDcuts, MCweight, CDweight, xlow, xup, nbins, N,blurb);
  return 0;
}

