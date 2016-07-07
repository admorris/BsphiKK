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

using namespace std;
void PlotAngAcc(string filename,string plotfilename)
{
  TFile* plotfile = TFile::Open(plotfilename.c_str() );
  TCanvas* can = (TCanvas*)plotfile->Get("acc_can" );
  string head = plotfilename.substr(0,plotfilename.size()-5);
  gStyle->SetOptStat(0);  
  TPad* pad;
  TCanvas* canv1d;
  TH1D* AccData;
  TH1D* AccProj;
  string histname[4] = {"mKK","phi","cosTheta","cosPsi"};
  string varname[4] = {"mKK","phi","ctheta_1","ctheta_2"};
  string axistitle[4] = { "#it{m}(#it{K}^{#plus}#it{K}^{#minus}) [MeV/#it{c}^{2}]"
                        , "#it{#Phi}"
                        , "cos #it{#theta_{1}}"
                        , "cos #it{#theta_{2}}"
                        };
  string unit[4] = { " MeV/#it{c}^{2}", "", "", "" };
  map<string,string> titlemap;
  float textsize = 0.055;
  for(int i = 0; i < 4; i++)
  {
    titlemap[varname[i]] = axistitle[i];
    pad = (TPad*)can->GetPrimitive(("acc_can_"+itoa(i+1)).c_str());
    AccData = (TH1D*)pad->GetPrimitive((histname[i]+"Acc"    ).c_str());
    AccProj = (TH1D*)pad->GetPrimitive((histname[i]+"AccProj").c_str());
    canv1d = new TCanvas("canvas","",1200,900);
    canv1d->cd();
    canv1d->SetFillColor(kWhite);
    pad = new TPad();
    // left right bottom top
    pad->SetMargin(0.15,0.05,0.15,0.05);
    pad->SetTicks(1,1);
    pad->SetFillColor(kWhite);
    pad->Draw();
    pad->cd();
    AccData = new TH1D(*AccData);
    AccData->SetMinimum(0);
    AccData->SetTitle("");
    AccData->GetXaxis()->SetTitle(axistitle[i].c_str());
    char ytitle[70];
    sprintf(ytitle,"Candidates / (%.1f%s)",AccData->GetBinWidth(1),unit[i].c_str());
    AccData->SetMaximum(1.3*AccData->GetMaximum());
    AccData->GetYaxis()->SetTitle(ytitle);
    AccData->SetLineColor(kBlack);
    AccData->SetLineWidth(1);
    AccData->SetMarkerSize(1);
    AccData->SetMarkerStyle(8);
    AccData->SetMarkerColor(kBlack);
    AccData->Draw("E1");
    AccProj = new TH1D(*AccProj);
    AccProj->SetLineColor(kRed);
    AccProj->SetLineWidth(2);
    AccProj->SetMarkerSize(0);
    AccProj->Draw("hist same c");
    AccData->GetXaxis()->SetTitleSize(textsize*1.2);
    AccData->GetYaxis()->SetTitleSize(textsize*1.2);
    AccData->GetXaxis()->SetLabelSize(textsize);
    AccData->GetYaxis()->SetLabelSize(textsize);
    AccData->GetXaxis()->SetTitleFont(132);
    AccData->GetYaxis()->SetTitleFont(132);
    AccData->GetXaxis()->SetLabelFont(132);
    AccData->GetYaxis()->SetLabelFont(132);
    AccData->GetYaxis()->CenterTitle();
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
    canv1d->SaveAs((head+"_"+varname[i]+"_proj.pdf").c_str());    
  }
  
  
  TFile* file = TFile::Open(filename.c_str());
  TTree* tree = (TTree*)file->Get("tuple");
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
    pad = new TPad("pad","",0,0,1,1);
    pad->SetMargin(0.12,0.08,0.12,0.08);
    pad->Draw();
    pad->cd();
    tree->Draw(comb[i].c_str(),"weight","COLZ");
    TH2F* graph = (TH2F*)pad->GetPrimitive("htemp");
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
    canv[i]->SaveAs((head+"_"+comb[i]+".pdf").c_str());
  }
  
  
//  tree->Draw();
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    PlotAngAcc("sampled_acceptance.root","acceptance.root");
    return 0;
  }
  else if(argc>3)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  PlotAngAcc((string)argv[1],(string)argv[2]);
  return 0;
}
