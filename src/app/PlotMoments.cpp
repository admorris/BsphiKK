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

void PlotMoments(std::string file, std::string massbranch, std::string ct2branch, std::string cuts, std::string weightname, std::string xtitle, std::string unit, std::string plotname, double xlow, double xup, int nbins, int max_order)
{
  auto tree = std::unique_ptr<TTree>(GetTree(file,cuts));
  std::vector<LegendreMomentPlot> plots = LegendreMomentPlot::FillPlotsFromTree("data",*tree,massbranch,ct2branch,weightname,xtitle,unit,plotname,xlow,xup,nbins,max_order,1.0);
  TCanvas allplots("allplots","",1200,1200);
  int ny = std::round(std::ceil(std::sqrt(max_order)));
  int nx = std::round(std::ceil(double(max_order)/ny));
  allplots.Divide(nx,ny);
  for(auto& lmplot : plots)
  {
    plotmaker plot(&lmplot.hist);
    plot.SetTitle(xtitle,unit);
    TCanvas* canvas = plot.Draw();
    std::stringstream ytitle;
    ytitle << lmplot.hist.GetName() << " / " << (xup-xlow)/nbins << " " << unit;
    lmplot.hist.GetYaxis()->SetTitle(ytitle.str().c_str());
    TLine zeroline(xlow,0,xup,0);
    zeroline.Draw();
    canvas->SaveAs((plotname+"_P"+std::to_string(lmplot.order)+".pdf").c_str());
    allplots.cd(&lmplot-&plots[0]+1);
    lmplot.hist.Draw("E1");
  }
  allplots.SaveAs((plotname+"_all.pdf").c_str());
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string file, mass, phi, ct1, ct2, cuts, weight, xtitle, unit, plot;
  double xlow, xup;
  int nbins, N;
  desc.add_options()
    ("help,H"    ,                                                                                 "produce help message")
    ("file,F"    , value<std::string>(&file  )->default_value("ntuples/BsphiKK_data_mvacut.root"), "data ntuple"         )
    ("mass"      , value<std::string>(&mass  )->default_value("BCON_KK_M_GeV"                   ), "mass branch"         )
    ("costheta"  , value<std::string>(&ct2   )->default_value("cos_theta2"                      ), "cosθ branch"         )
    ("cuts,C"    , value<std::string>(&cuts  )->default_value(""                                ), "cuts"                )
    ("weight,W"  , value<std::string>(&weight)->default_value(""                                ), "weight"              )
    ("title,T"   , value<std::string>(&xtitle)->default_value("#it{m}(#it{K^{+}K^{#minus}})"    ), "x-axis title"        )
    ("unit,U"    , value<std::string>(&unit  )->default_value("GeV/#it{c}^{2}"                  ), "x-axis unit"         )
    ("plot,O"    , value<std::string>(&plot  )->default_value("LegendreMoments"                 ), "output plot filename")
    ("upper,u"   , value<double     >(&xup   )->default_value(1.80                              ), "x-axis upper limit"  )
    ("lower,l"   , value<double     >(&xlow  )->default_value(1.05                              ), "x-axis lower limit"  )
    ("bins,b"    , value<int        >(&nbins )->default_value(25                                ), "number of bins"      )
    ("max-order" , value<int        >(&N     )->default_value(5                                 ), "max polynomial order")
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help") || N < 0 || nbins < 0 || xlow >= xup)
  {
    std::cout << desc << std::endl;
    return 1;
  }
  std::cout << "Entering main function" << std::endl;
  PlotMoments(file, mass, ct2, cuts, weight, xtitle, unit, plot, xlow, xup, nbins, N);
  return 0;
}

