#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TPad.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TGraph.h"

#include "itoa.h"

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using std::map;
void PlotAngAcc(string filename)
{
  TFile* file = TFile::Open(filename.c_str());
  TTree* dataacctree = (TTree*)file->Get("dataacctuple");
  TTree* sampledtree = (TTree*)file->Get("sampledtuple");
  gStyle->SetOptStat(0);
  string varname[4] = {"mKK","phi","ctheta_1","ctheta_2"};
  string axistitle[4] = { "#it{m}(#it{K}^{#plus}#it{K}^{#minus}) [MeV/#it{c}^{2}]"
                        , "#it{#Phi}"
                        , "cos #it{#theta_{1}}"
                        , "cos #it{#theta_{2}}"
                        };
  string unit[4] = { " MeV/#it{c}^{2}", "", "", "" };
  double minima[4] = {0.988, -M_PI, -1, -1};
  double maxima[4] = {1.800, +M_PI, +1, +1};
  int nbins[4] = {30, 30, 30, 30};
  map<string,string> titlemap;
  float textsize = 0.055;
  cout << "Drawing 1D plots" << endl;
  for(int i = 0; i < 4; i++)
  {
    titlemap[varname[i]] = axistitle[i];
    TCanvas canv1d((varname[i]+"1Dplot").c_str(),"",1200,900);
    TH1D AccData((varname[i]+"AccData").c_str(),"",nbins[i],minima[i],maxima[i]);
    TH1D AccProj((varname[i]+"AccProj").c_str(),"",nbins[i],minima[i],maxima[i]);
    canv1d.SetFillColor(kWhite);
    TPad pad;
    // left right bottom top
    pad.SetMargin(0.15,0.05,0.15,0.05);
    pad.SetTicks(1,1);
    pad.SetFillColor(kWhite);
    pad.Draw();
    pad.cd();
    dataacctree->Draw((varname[i]+">>"+AccData.GetName()).c_str(),"weight","E1");
    AccData.Scale(1./AccData.Integral());
    AccData.SetMinimum(0);
    AccData.SetTitle("");
    AccData.GetXaxis()->SetTitle(axistitle[i].c_str());
    char ytitle[70];
    sprintf(ytitle,"Candidates / (%.3f%s)",AccData.GetBinWidth(1),unit[i].c_str());
    AccData.GetYaxis()->SetTitle(ytitle);
    AccData.SetLineColor(kBlack);
    AccData.SetLineWidth(1);
    AccData.SetMarkerSize(1);
    AccData.SetMarkerStyle(8);
    AccData.SetMarkerColor(kBlack);
    sampledtree->Draw((varname[i]+">>"+AccProj.GetName()).c_str(),"weight","HIST SAME C");
    AccProj.Scale(1./AccProj.Integral());
    AccProj.SetLineColor(kRed);
    AccProj.SetLineWidth(2);
    AccProj.SetMarkerSize(0);
    AccData.SetMaximum(1.3*AccData.GetMaximum());
    AccData.GetXaxis()->SetTitleSize(textsize*1.2);
    AccData.GetYaxis()->SetTitleSize(textsize*1.2);
    AccData.GetXaxis()->SetLabelSize(textsize);
    AccData.GetYaxis()->SetLabelSize(textsize);
    AccData.GetXaxis()->SetTitleFont(132);
    AccData.GetYaxis()->SetTitleFont(132);
    AccData.GetXaxis()->SetLabelFont(132);
    AccData.GetYaxis()->SetLabelFont(132);
    AccData.GetYaxis()->CenterTitle();
    double _blurbx        = 0.75;
    double _blurby        = 0.80;
    string _blurbtext     = "#splitline{LHCb}{#scale[0.75]{Preliminary}}";
    TLatex _blurb(_blurbx,_blurby,_blurbtext.c_str());
    _blurb.SetTextFont(132);
    _blurb.SetTextColor(1);
    _blurb.SetNDC(kTRUE);
    _blurb.SetTextAlign(11);
    _blurb.SetTextSize(0.07);
    _blurb.Draw();
    canv1d.SaveAs(("acceptance_"+varname[i]+"_proj.pdf").c_str());
  }
  TCanvas* canv[6];
  string comb[6] = {"ctheta_1:ctheta_2","ctheta_1:phi","ctheta_2:phi","phi:mKK","ctheta_1:mKK","ctheta_2:mKK"};
  for(int i = 0; i < 6; i++)
  {
    canv[i] = new TCanvas(("projection"+comb[i]).c_str(),"",500,500);
  }
  for(int i = 0; i < 6; i++)
  {
    cout << "Drawing " << comb[i] << endl;
    canv[i]->cd();
    string::size_type separator = comb[i].find(':');
    TPad pad("pad","",0,0,1,1);
    pad.SetMargin(0.12,0.08,0.12,0.08);
    pad.Draw();
    pad.cd();
    sampledtree->Draw(comb[i].c_str(),"weight","COLZ");
    TH2F* graph = (TH2F*)pad.GetPrimitive("htemp");
    graph->SetTitle("");
    graph->GetXaxis()->SetTitleSize(textsize);
    graph->GetXaxis()->SetLabelSize(textsize);
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->SetTitleSize(textsize);
    graph->GetYaxis()->SetLabelSize(textsize);
    graph->GetYaxis()->CenterTitle();
    if(comb[i].find("mKK")!=string::npos)
    {
      graph->GetXaxis()->SetNdivisions(505);
    }
    cout << titlemap[comb[i].substr(0,separator)] << endl;
    graph->GetYaxis()->SetTitle(titlemap[comb[i].substr(0,separator)].c_str());
    graph->GetXaxis()->SetTitle(titlemap[comb[i].substr(separator+1,comb[i].size())].c_str());
    cout << "Fetched graph" << endl;
    std::replace(comb[i].begin(),comb[i].end(), ':', '-');
    canv[i]->SaveAs(("acceptance_"+comb[i]+".pdf").c_str());
  }
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    PlotAngAcc("sampled_acceptance.root");
    return 0;
  }
  else if(argc>2)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  PlotAngAcc((string)argv[1]);
  return 0;
}
