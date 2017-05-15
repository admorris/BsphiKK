#include "CutEff.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TGraphErrors.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TPad.h"
#include "TStyle.h"
#include "TAxis.h"
#include "GetTree.h"
using namespace std;
template<typename T>
string tostr(T value)
{
  stringstream s;
  s << setprecision(3) << fixed << value;
  return s.str();
}
void DrawGraph(string name, TGraphErrors* graph)
{
  TCanvas* canvas = new TCanvas(name.c_str(),"",1200,900);
  canvas->SetFillColor(kWhite);
  canvas->cd();
  TPad* pad = new TPad();
  // left right bottom top
  pad->SetMargin(0.15,0.05,0.15,0.05);
  pad->SetTicks(1,1);
  pad->SetFillColor(kWhite);
  pad->Draw();
  pad->cd();
  TGraphErrors* oldgraph = graph;
  graph = new TGraphErrors(*graph);
  delete oldgraph;
  float textsize = 0.055;
  graph->SetMinimum(0);
  graph->SetMaximum(100);
  graph->SetTitle("");
  graph->GetXaxis()->SetTitle("#it{m}(#it{K^{#plus}K^{#minus}}) [MeV/#it{c}^{2}]");
  graph->GetYaxis()->SetTitle("Efficiency [%]");
  graph->SetLineColor(kBlack);
  graph->SetLineWidth(1);
  graph->SetMarkerSize(1);
  graph->SetMarkerStyle(8);
  graph->SetMarkerColor(kBlack);
  graph->Draw("AP");
  graph->GetXaxis()->SetTitleSize(textsize*1.2);
  graph->GetYaxis()->SetTitleSize(textsize*1.2);
  graph->GetXaxis()->SetLabelSize(textsize);
  graph->GetYaxis()->SetLabelSize(textsize);
  graph->GetXaxis()->SetTitleFont(132);
  graph->GetYaxis()->SetTitleFont(132);
  graph->GetXaxis()->SetLabelFont(132);
  graph->GetYaxis()->SetLabelFont(132);
  graph->GetYaxis()->CenterTitle();
  /*
  double _blurbx        = 0.75;
  double _blurby        = 0.80;
  string _blurbtext     = "#splitline{LHCb}{#scale[0.75]{Preliminary}}";
  TLatex* _blurb = new TLatex(_blurbx,_blurby,_blurbtext.c_str());
  _blurb->SetTextFont(132);
  _blurb->SetTextColor(1);
  _blurb->SetNDC(kTRUE);
  _blurb->SetTextAlign(11);
  _blurb->SetTextSize(0.07);
  _blurb->Draw();
  */
  canvas->SaveAs((name+".pdf").c_str());
  canvas->Write();
}
struct TrigEffPlot
{
  TrigEffPlot(string nam,int n,string b, string a) : name(nam), befcut(b), aftcut(a)
  {
    y = new double[n];
    ye = new double[n];
  }
  string name;
  string befcut;
  string aftcut;
  double* y;
  double* ye;
};
void PlotTrigEff(string filename,string plotname)
{
  string L0[]={"B_s0_L0HadronDecision_TOS", "B_s0_L0Global_TIS"};
  string Hlt1="B_s0_Hlt1TrackAllL0Decision_TOS";
  string Hlt2[]={"B_s0_Hlt2Topo3BodyBBDTDecision_TOS", "B_s0_Hlt2Topo4BodyBBDTDecision_TOS"};
  const unsigned int nbins = 20;
  double mKKlo = 990, mKKhi = 1800, binw = (mKKhi-mKKlo)/nbins;
  double mKK[nbins], mKKE[nbins];
  TTree* intree = GetTree(filename);
  TFile outputfile("TrigEffPlots.root","RECREATE");
  vector<TrigEffPlot> Plots;
  Plots.push_back(TrigEffPlot("HadronTOS",nbins,"",    L0[0]                                                       ));
  Plots.push_back(TrigEffPlot("GlobalTIS",nbins,"",               L0[1]                                            ));
  Plots.push_back(TrigEffPlot("Level0Tot",nbins,"","("+L0[0]+"||"+L0[1]+")"                                        ));
  Plots.push_back(TrigEffPlot("Hlt1Total",nbins,"&& ("+L0[0]+"||"+L0[1]+")",   Hlt1                                ));
  Plots.push_back(TrigEffPlot("Topo3Body",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",   Hlt2[0]                 ));
  Plots.push_back(TrigEffPlot("Topo4Body",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",                Hlt2[1]    ));
  Plots.push_back(TrigEffPlot("Hlt2Total",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",   Hlt2[0]+"||"+Hlt2[1]    ));
  Plots.push_back(TrigEffPlot("TrigTotal",nbins,"","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")&&("+Hlt2[0]+"||"+Hlt2[1]+")"));
  for(auto Plot : Plots)
  {
    for(unsigned int ibin = 0; ibin < nbins; ibin++)
    {
      double binlo = mKKlo+ibin*binw, binhi = mKKlo+(ibin+1)*binw;
      mKK[ibin] = (binhi+binlo)/2.0;
      mKKE[ibin] = binw/2.0;
      string mKKcut = "(KK_M>"+tostr(binlo)+"&&KK_M<"+tostr(binhi)+")";
      CutResult_t Result = CutEff(intree,mKKcut+Plot.befcut,Plot.aftcut);
      Plot.y [ibin] = Result.GetEff()   *100.0;
      Plot.ye[ibin] = Result.GetEffErr()*100.0;
    }
    DrawGraph(plotname+Plot.name,new TGraphErrors(nbins,mKK,Plot.y,mKKE,Plot.ye));
  }
  outputfile.Close();
}
int main(int argc, char* argv[])
{
  if(argc!=3)
  {
    cout << "Usage: " << argv[0] << " <filename> <plot name prefix>" << endl;
    return 1;
  }
  PlotTrigEff(argv[1],argv[2]);
  return 0;
}
