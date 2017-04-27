// Standard headers
#include <string>
#include <iostream>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"
// Custom headers
#include "plotmaker.h"
#include "annotation.h"
#include "MakeBranchPlot.h"
using std::string;
using std::cout;
using std::endl;
void AnnotateBranch(string filename, string branchname, string xtitle, string unit, string plotname, string cuts, string weight, double xlow, double xup, int nbins, string overlay, double scale, bool noblurb)
{
  using namespace std;
  TH1D* frame = MakeBranchPlot(filename, branchname, cuts, weight, xlow, xup, nbins);
  unsigned int n;
  annotation** particles;
/*****************************************************************************/
  annotation* KKpeaks[] =
  {
    resonance(1019.461,"  #it{#phi}"        )
  , resonance(1864.84 ,"#it{D}^{0}"         )
  , resonance(1525    ,"#it{f'}_{2}(1525)"  )
  , resonance(3096.916,"#it{J/#psi}"        )
  , resonance(3414.75 ,"#it{#chi}_{#it{c}0}")
  , resonance(3556.20 ,"#it{#chi}_{#it{c}2}")
  , resonance(3686.109," #psi(2S)"          )
  };
  annotation* beautymesons[] =
  {
    resonance(5279.61,"#it{B}^{0}")
  , resonance(5366.79,"#it{B_{s}}^{0}")
  };
  annotation* beautybaryons[] =
  {
    resonance(5619.51,"#it{#Lambda_{b}}^{0}")
  };
  annotation* charmmesons[] =
  {
    resonance(1869.61,"#it{D}^{#plus}"    )
  , resonance(1968.30,"#it{D_{s}}^{#plus}")
  };
  annotation* charmantimesons[] =
  {
    resonance(1869.61,"#it{D}^{#minus}"    )
  , resonance(1968.30,"#it{D_{s}}^{#minus}")
  };
  annotation* charmbaryons[] =
  {
    resonance(2286.46,"#it{#Lambda_{c}}^{#plus}")
  };
  annotation* charmantibaryons[] =
  {
    resonance(2286.46,"#it{#bar{#Lambda}_{c}}^{#minus}")
  };
  annotation* strangebaryons[] =
  {
    resonance(1519.5,"#it{#Lambda}(1520)^{0}")
  };
/*****************************************************************************/
  bool go = true;
  if(branchname == "KK_M")
  {
    if(xlow>1020)
    {
      KKpeaks[0] = resonance(1100, "  #it{#phi} tail", false);
    }
    particles = KKpeaks;
    n = sizeof(KKpeaks)/sizeof(annotation*);
  }
  else if(branchname == "phipiplusM" || branchname == "phiKplusM")
  {
    particles = charmmesons;
    n = sizeof(charmmesons)/sizeof(annotation*);
  }
  else if(branchname == "phipiminusM" || branchname == "phiKminusM")
  {
    particles = charmantimesons;
    n = sizeof(charmantimesons)/sizeof(annotation*);
  }
  else if(branchname == "phipM")
  {
    particles = charmbaryons;
    n = sizeof(charmbaryons)/sizeof(annotation*);
  }
  else if(branchname == "KpM")
  {
    particles = strangebaryons;
    n = sizeof(strangebaryons)/sizeof(annotation*);
  }
  else if(branchname == "phipbarM")
  {
    particles = charmantibaryons;
    n = sizeof(charmantibaryons)/sizeof(annotation*);
  }
  else if(branchname == "phiKpiM" || branchname == "phipipiM" || branchname == "phiphimass" || branchname == "phikstmass")
  {
    particles = beautymesons;
    n = sizeof(beautymesons)/sizeof(annotation*);
  }
  else if(branchname == "phiKpM")
  {
    particles = beautybaryons;
    n = sizeof(beautybaryons)/sizeof(annotation*);
  }
  else
  {
    cout << "No stored list of resonances for branch name " << branchname << endl;
    go=false;
  }
  plotmaker plotter(frame);
  if(noblurb) plotter.SetBlurb("");
//  frame->SetDrawOption("E1");
  plotter.SetTitle(xtitle,unit);
  TCanvas* canv = plotter.Draw("E1");
  frame->SetMaximum(frame->GetMaximum()*1.3);
  if(overlay != "")
  {
    TH1D* overlayframe = MakeBranchPlot(overlay, branchname, cuts, weight, xlow, xup, nbins);
    overlayframe->Scale(scale);
    overlayframe->SetLineWidth(0);
    overlayframe->SetFillColor(kOrange);
    overlayframe->Draw("same");
  }
  TLatex* label;
  TLine* line;
  double range = xup-xlow;
  // Which bin to get the height of
  int bin;
  if(go)
  {
    // Loop through the particles
    for(unsigned int i = 0; i < n; i++)
    {
      line  = &(particles[i]->line);
      label = &(particles[i]->label);
      cout << "Labelling particle at " << line->GetX1() << " MeV" << endl;
      // Find the right bin for the line
      bin=0;
      for(int j = 1; j < nbins && frame->GetBinLowEdge(j+1) < line->GetX1(); j++)
      {
        bin = j+1;
      }
      // Check if the particle is in the range of the plot, and the line would be big enough
      if(line->GetX1()>xlow && line->GetX1()<xup && frame->GetBinContent(bin)/frame->GetMaximum() > 0.05)
      {
        cout << "Bin " << bin << endl;
        // Scale up the line
        line->SetY2(line->GetY2()*frame->GetBinContent(bin));
        line->Draw();
        // Put the label above surrounding error bars
        label->SetX(label->GetX()-2*range/100);
        double ys[] = {frame->GetBinContent(TMath::Abs(bin-1)),frame->GetBinContent(bin),frame->GetBinContent(bin+1)};
        label->SetY(label->GetY()*(*max_element(ys,ys+3))+0.05*frame->GetMaximum()+frame->GetBinErrorUp(bin));
        label->Draw();
      }
      else
      {
        cout << "Out of range" << endl;
      }
    }
  }
  canv->SaveAs((plotname+".pdf").c_str());
}
int main(int argc, char* argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  string file, branch, cuts, xtitle, unit, plot, weight, overlay;
  double xlow, xup, scale;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                              "produce help message"                )
    ("noblurb" ,                                                                              "no blurb"                            )
    ("file,F"  , value<string>(&file   )->default_value("ntuples/BsphiKK_data_mvaVars.root"), "data file"                           )
    ("branch,B", value<string>(&branch )->default_value("KK_M"                             ), "branch to plot"                      )
    ("weight,W", value<string>(&weight )->default_value(""                                 ), "weighting variable"                  )
    ("cuts,C"  , value<string>(&cuts   )->default_value(""                                 ), "optional cuts"                       )
    ("title,T" , value<string>(&xtitle )->default_value("#it{m}(#it{K^{#plus}K^{#minus}})" ), "x-axis title"                        )
    ("unit,U"  , value<string>(&unit   )->default_value("MeV/#it{c}^{2}"                   ), "unit"                                )
    ("plot,O"  , value<string>(&plot   )->default_value("plottedbranch"                    ), "output plot filename"                )
    ("upper,u" , value<double>(&xup    )->default_value(4000                               ), "branch upper limit"                  )
    ("lower,l" , value<double>(&xlow   )->default_value(900                                ), "branch lower limit"                  )
    ("bins,b"  , value<int   >(&nbins  )->default_value(50                                 ), "number of bins"                      )
    ("overlay" , value<string>(&overlay)->default_value(""                                 ), "file to overlay the same branch from")
    ("scale"   , value<double>(&scale  )->default_value(1                                  ), "scale factor for the overlaid branch")
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
  AnnotateBranch(file,branch,xtitle,unit,plot,cuts,weight,xlow,xup,nbins,overlay,scale,vmap.count("noblurb"));
  return 0;
}
