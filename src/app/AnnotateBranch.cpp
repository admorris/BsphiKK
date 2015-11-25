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
#include "annotation.h"
#include "maxOfFive.h"
#include "maxOfThree.h"

void AnnotateBranch(string filename, string branchname, string xtitle, string unit, string plotname, string cuts, string weight, double xlow, double xup, int nbins)
{
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  using namespace RooFit;
  RooRealVar* x = new RooRealVar(branchname.c_str(),xtitle.c_str(),xlow,xup);
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
  frame->SetMinimum(0);
  RooHist* hist = frame->getHist();
  
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
  else if(branchname == "phipM" || branchname == "KpM")
  {
    particles = charmbaryons;
    n = sizeof(charmbaryons)/sizeof(annotation*);    
  }
  else if(branchname == "phipbarM")
  {
    particles = charmantibaryons;
    n = sizeof(charmantibaryons)/sizeof(annotation*);    
  }
  else if(branchname == "phiKpiM" || branchname == "phipipiM")
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
  plotter.SetTitle(xtitle,unit);
  TCanvas* canv = plotter.Draw();
  frame->SetMaximum(frame->GetMaximum()*1.1);
  TLatex* label;
  TLine* line;
  // Get number of bins in hist
  int N = hist->GetN();
  // Arrays to store Y-values and upper errors
  double* height;
  double* errorbar;
  double range = xup-xlow;
  // Which bin to get the height of
  int bin;
  if(go)
  {
    height = hist->GetY();
    errorbar = hist->GetEYhigh();
    // Loop through the particles
    for(unsigned int i = 0; i < n; i++)
    {
      line = &(particles[i]->line);
      label = &(particles[i]->label);
        cout << "Labelling particle at " << line->GetX1() << " MeV" << endl;
      // Find the right bin for the line
      bin=0;
      for(int j = 0; j < N-1 && hist->GetX()[j]+hist->GetErrorXhigh(j) < line->GetX1(); j++)
      {
        bin = j+1;
      }
      // Check if the particle is in the range of the plot, and the line would be big enough
      if(line->GetX1()>xlow && line->GetX1()<xup && height[bin]/frame->GetMaximum() > 0.05)
      {
        cout << "Bin " << bin << endl;        
        // Scale up the line
        line->SetY2(line->GetY2()*height[bin]);
        line->Draw();
        // Put the label above surrounding error bars
        label->SetX(label->GetX()-2*range/100);
//        label->SetY(label->GetY()*maxOfFive(height[bin-2],height[bin-1],height[bin],height[bin+1],height[bin+2])+0.05*frame->GetMaximum()+errorbar[bin]);
        label->SetY(label->GetY()*maxOfThree(height[TMath::Abs(bin-1)],height[bin],height[bin+1])+0.05*frame->GetMaximum()+errorbar[bin]);
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
  std::string file, branch, cuts, xtitle, unit, plot, weight;
  double xlow, xup;
  int nbins;
  desc.add_options()
    ("help,H"  ,                                                                                  "produce help message"                      )
    ("file,F"  , value<std::string>(&file  )->default_value("ntuples/BsphiKK_data_mvaVars.root"), "set data file"                             )
    ("branch,B", value<std::string>(&branch)->default_value("KK_M"                             ), "set branch to plot"                        )
    ("weight,W", value<std::string>(&weight)->default_value("Nsig_sw"                          ), "set weighting variable"                    )
    ("cuts,C"  , value<std::string>(&cuts  )->default_value(""                                 ), "set optional cuts"                         )
    ("title,T" , value<std::string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}})" ), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"  , value<std::string>(&unit  )->default_value("MeV/#it{c}^{2}"                   ), "set unit (takes ROOT LaTeX format)"        )
    ("plot,O"  , value<std::string>(&plot  )->default_value("plottedbranch"                    ), "set output plot filename"                  )
    ("upper,u" , value<double     >(&xup   )->default_value(4000                               ), "set branch upper limit"                    )
    ("lower,l" , value<double     >(&xlow  )->default_value(900                                ), "set branch lower limit"                    )
    ("bins,b"  , value<int        >(&nbins )->default_value(50                                 ), "set number of bins"                        )
    
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
  AnnotateBranch(file,branch,xtitle,unit,plot,cuts,weight,xlow,xup,nbins);
  return 0;
}