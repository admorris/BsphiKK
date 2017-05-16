// Standard headers
#include <string>
#include <vector>
#include <iostream>
// ROOT headers
#include "TAxis.h"
#include "TGraph.h"
#include "TLine.h"
#include "TLatex.h"
// BOOST headers
#include "boost/program_options.hpp"
// Custom headers
#include "GetTree.h"
#include "CutEff.h"
#include "substitute.h"
void ROCcurve(std::string Sfilename, std::string Bfilename, double xmin, double xmax, double ymin, double ymax, std::string expression, double varmin, double varmax, std::string cuts, std::string weight, std::string blurbtext, int nsteps, std::string plotname, std::string comparison)
{
  TTree* Stree = GetTree(Sfilename);
  TTree* Btree = GetTree(Bfilename);
  std::vector<double> sigeff;
  std::vector<double> bkgrej;
  for(double var = varmin; var <= varmax+0.0001; var+=(varmax-varmin)/(double)nsteps)
  {
    std::string cut = substitute(expression, "{}", std::to_string(var));
    std::cout << "\nApplying cut " << cut << "\n";
    sigeff.push_back(CutEff(Stree, cuts, cut).GetEff());
    bkgrej.push_back(1-CutEff(Btree, cuts, cut).GetEff());
  }
  assert(sigeff.size() == bkgrej.size());
  TCanvas canv;
  canv.SetMargin(0.1,0.02,0.11,0.02);
  // Set up the axes for drawing
  double xdummy[2] = {xmin,xmax};
  double ydummy[2] = {ymin,ymax};
  TGraph dummyaxes(2,xdummy,ydummy);
  dummyaxes.SetTitle("");
  TAxis* Xaxis = dummyaxes.GetXaxis(),
       * Yaxis = dummyaxes.GetYaxis();
  Xaxis->SetTitle("Background rejection");
  Yaxis->SetTitle("Signal efficiency");
  for(TAxis* axis : {Xaxis,Yaxis})
  {
    axis->SetTitleSize(0.05);
    axis->SetLabelSize(0.05);
    axis->SetTitleFont(132);
    axis->SetLabelFont(132);
  }
  dummyaxes.Draw("AP");
  Xaxis->SetRangeUser(xmin,xmax);
  Yaxis->SetRangeUser(ymin,ymax);
  TLine x90(0.9,ymin,0.9,ymax), y90(xmin,0.9,xmax,0.9);
  for(TLine* line: {&x90, &y90,})
  {
    if(line->GetX1()>=xmin && line->GetX2()<=xmax && line->GetY1()>=ymin && line->GetY2()<=ymax)
    {
      line->SetLineStyle(kDashed);
      line->SetLineWidth(2);
      line->SetLineColor(kBlack);
      line->Draw();
    }
  }
  // Make the ROC curve
  TGraph roc_curve(sigeff.size(), bkgrej.data(), sigeff.data());
  roc_curve.SetLineStyle(kSolid);
  roc_curve.SetLineWidth(2);
  roc_curve.SetLineColor(kRed);
  roc_curve.SetMarkerStyle(20);
  roc_curve.SetMarkerSize(0.5);
  roc_curve.Draw("PLsame");
  // Draw comparison point
  double xcomp[1], ycomp[1];
  if(comparison != "")
  {
    xcomp[0] = 1-CutEff(Btree, cuts, comparison).GetEff();
    ycomp[0] = CutEff(Stree, cuts, comparison).GetEff();
  }
  TGraph comp_graph(1,xcomp,ycomp);
  if(comparison != "")
    comp_graph.Draw("*same");
  // Draw blurb
  TLatex blurb;
  if(blurbtext!="")
  {
    blurb.DrawLatex(xmin+0.05*(xmax-xmin),ymin+0.15*(ymax-ymin),blurbtext.c_str());
  }
  canv.SaveAs((plotname+".pdf").c_str());
}
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  std::string Sfile, Bfile, cuts, weight, expr, blurb, plot, comp;
  double xmin, xmax, ymin, ymax, varmin, varmax;
  int nsamples;
  desc.add_options()
    ("help,H" ,                                                                              "produce help message"                                            )
    ("blurb"  , value<std::string>(&blurb   )                                              , "blurb text"                                                      )
    ("expr"   , value<std::string>(&expr    )->default_value("mlp>{}"                     ), "cut expression. '{}' will be substituted with the variable value")
    ("comp"   , value<std::string>(&comp    )                                              , "optional comparison expression to draw as a single point"        )
    ("varmax" , value<double     >(&varmax  )->default_value(1.0                          ), "variable upper limit"                                            )
    ("varmin" , value<double     >(&varmin  )->default_value(0.0                          ), "variable lower limit"                                            )
    ("samples", value<int        >(&nsamples)->default_value(10                           ), "number of samples"                                               )
    ("Sfile"  , value<std::string>(&Sfile   )->default_value("ntuples/BsphiKK_MC_mva.root"), "signal file"                                                     )
    ("Bfile"  , value<std::string>(&Bfile   )->default_value("ntuples/LbphiKp_MC_mva.root"), "background file"                                                 )
    ("cuts"   , value<std::string>(&cuts    )->default_value(""                           ), "optional cuts"                                                   )
    ("plot"   , value<std::string>(&plot    )->default_value("roc_curve"                  ), "output plot filename"                                            )
    ("xmax"   , value<double     >(&xmax    )->default_value(1.0                          ), "x axis upper limit"                                              )
    ("xmin"   , value<double     >(&xmin    )->default_value(0.0                          ), "x axis lower limit"                                              )
    ("ymax"   , value<double     >(&ymax    )->default_value(1.0                          ), "y axis upper limit"                                              )
    ("ymin"   , value<double     >(&ymin    )->default_value(0.0                          ), "y axis lower limit"                                              )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }
  ROCcurve(Sfile, Bfile, xmin, xmax, ymin, ymax, expr, varmin, varmax, cuts, weight, blurb, nsamples, plot, comp);
  return 0;
}
