#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
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
  TFile* plotfile = new TFile(plotfilename.c_str() );
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
  map<string,string> titlemap;
  float textsize = 0.05;
  for(int i = 0; i < 4; i++)
  {
    titlemap[varname[i]] = axistitle[i];
    pad = (TPad*)can->GetPrimitive(("acc_can_"+itoa(i+1)).c_str());
    AccData = (TH1D*)pad->GetPrimitive((histname[i]+"Acc"    ).c_str());
    AccProj = (TH1D*)pad->GetPrimitive((histname[i]+"AccProj").c_str());
    canv1d = new TCanvas();
    canv1d->cd();
    pad = new TPad();
    pad->SetMargin(0.05,0.05,0.05,0.2);
    pad->Draw();
    pad->cd();
    AccData->SetMinimum(0);
    AccData->SetTitle("");
    AccData->GetXaxis()->SetTitle(axistitle[i].c_str());
    AccData->Draw();
    AccProj->Draw("same");
    AccData->GetXaxis()->SetTitleSize(textsize);
    AccData->GetXaxis()->SetLabelSize(textsize);
    AccData->GetXaxis()->CenterTitle();
    AccData->GetYaxis()->SetLabelSize(textsize);
    canv1d->SaveAs((head+"_"+varname[i]+"_proj.pdf").c_str());    
  }
  
  
  TFile* file = new TFile(filename.c_str());
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
