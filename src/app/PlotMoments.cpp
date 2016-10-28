// Standard C++
#include <iostream>
#include <sstream>
#include <vector>
// ROOT
#include "TLine.h"
// BOOST
#include "boost/program_options.hpp"
// GSL
#include <gsl/gsl_sf_legendre.h>
// Custom
#include "GetTree.h"
#include "plotmaker.h"
#include "itoa.h"

struct LegendreMomentPlot
{
  LegendreMomentPlot(int _order, int nbins, double xlow, double xup) : order(_order)
  {
    string name = "<P_{" + itoa(order) + "}>";
    cout << name << endl;
    hist = TH1D(name.c_str(),"",nbins,xlow,xup);
  }
  int order;
  TH1D hist;
};

void PlotMoments(string file, string massbranch, string ct2branch, string cuts, string weightname, string xtitle, string unit, string plotname, double xlow, double xup, int nbins, int max_order)
{
  vector<LegendreMomentPlot> plots;
  for(int order = 0; order <= max_order; order++)
    plots.push_back(LegendreMomentPlot(order, nbins, xlow, xup));
  cout << "Booked " << plots.size() << " histograms" << endl;
  TTree* tree = GetTree(file,cuts);
  float weight = 1;
  if(weightname != "")
    tree->SetBranchAddress(weightname.c_str(),&weight);
  double mass; tree->SetBranchAddress(massbranch.c_str(),&mass);
  double ct2 ; tree->SetBranchAddress(ct2branch .c_str(),&ct2 );
  for(int i = tree->GetEntries(); i-->0;)
  {
    tree->GetEntry(i);
    for(auto& lmplot : plots)
    {
      double p = gsl_sf_legendre_Pl(lmplot.order, ct2);
      lmplot.hist.Fill(mass,p*weight);
    }
  }
  for(auto& lmplot : plots)
  {
    plotmaker plot(&lmplot.hist);
    plot.SetTitle(xtitle,unit);
    TCanvas* canvas = plot.Draw();
    stringstream ytitle;
    ytitle << lmplot.hist.GetName() << " / " << (xup-xlow)/nbins << " " << unit;
    lmplot.hist.GetYaxis()->SetTitle(ytitle.str().c_str());
    TLine zeroline(xlow,0,xup,0);
    zeroline.Draw();
    canvas->SaveAs((plotname+"_P"+itoa(lmplot.order)+".pdf").c_str());
  }
}

int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  string file, mass, phi, ct1, ct2, cuts, weight, xtitle, unit, plot;
  double xlow, xup;
  int nbins, N;
  desc.add_options()
    ("help,H"    ,                                                                            "produce help message")
    ("file,F"    , value<string>(&file  )->default_value("ntuples/BsphiKK_data_mvacut.root"), "data ntuple"         )
    ("mass"      , value<string>(&mass  )->default_value("KK_M"                            ), "mass branch"         )
    ("costheta"  , value<string>(&ct2   )->default_value("cos_theta2"                      ), "cosθ branch"         )
    ("cuts,C"    , value<string>(&cuts  )->default_value("KK_M<1800"                       ), "cuts"                )
    ("weight,W"  , value<string>(&weight)->default_value(""                                ), "weight"              )
    ("title,T"   , value<string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}})"), "x-axis title"        )
    ("unit,U"    , value<string>(&unit  )->default_value("MeV/#it{c}^{2}"                  ), "x-axis unit"         )
    ("plot,O"    , value<string>(&plot  )->default_value("LegendreMoments"                 ), "output plot filename")
    ("upper,u"   , value<double>(&xup   )->default_value(1800                              ), "x-axis upper limit"  )
    ("lower,l"   , value<double>(&xlow  )->default_value(980                               ), "x-axis lower limit"  )
    ("bins,b"    , value<int   >(&nbins )->default_value(41                                ), "number of bins"      )
    ("max-order" , value<int   >(&N     )->default_value(5                                 ), "max polynomial order")
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
  PlotMoments(file, mass, ct2, cuts, weight, xtitle, unit, plot, xlow, xup, nbins, N);
  return 0;
}
