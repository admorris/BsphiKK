#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TGraph.h"
#include <string>
#include <iostream>
#include "gradients.h"

using namespace std;
void PlotBackground(string filename)
{
  heatmapgradient();
  gStyle->SetOptStat(0);
  TFile* file = TFile::Open(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  TCanvas* can[4];
  string bra[4] = {"cos_theta1"
                  ,"cos_theta2"
                  ,"Phi_angle"
                  ,"KK_M"};
  string tit[4] = {"cos#theta_{1}"
                  ,"cos#theta_{2}"
                  ,"#Phi"
                  ,"m(K^{#plus}K^{#minus})"};
  string opt[4] = {"10,-1,1"
                  ,"10,-1,1"
                  ,"10,-3.142,3.142"
                  ,"10,988,1800"};
  for(int i = 0; i < 4; i++)
  {
    can[i] = new TCanvas(("projection"+bra[i]).c_str(),"",500,500);
    can[i]->Draw();
    can[i]->cd();
    tree->Draw((bra[i]+">>htemp("+opt[i]+")").c_str());
    TH1F* graph = (TH1F*)can[i]->GetPrimitive("htemp");
    graph->SetTitle("");
    graph->GetXaxis()->SetTitle(tit[i].c_str());
    graph->SetMinimum(0);
    can[i]->SaveAs((bra[i]+".pdf").c_str());
  }
  TCanvas* canv[6];
  string comb[6] = {"cos_theta1:cos_theta2"
                   ,"cos_theta1:Phi_angle"
                   ,"cos_theta2:Phi_angle"
                   ,"Phi_angle:KK_M"
                   ,"cos_theta1:KK_M"
                   ,"cos_theta2:KK_M"};
  string opts[6] = {"5,-1,1,5,-1,1"
                   ,"5,-3.142,3.142,5,-1,1"
                   ,"5,-3.142,3.142,5,-1,1"
                   ,"5,988,1800,5,-3.142,3.142"
                   ,"5,988,1800,5,-1,1"
                   ,"5,988,1800,5,-1,1"};
  string ytit[6] = {"cos#theta_{1}"
                   ,"cos#theta_{1}"
                   ,"cos#theta_{2}"
                   ,"#Phi"
                   ,"cos#theta_{1}"
                   ,"cos#theta_{2}"};
  string xtit[6] = {"cos#theta_{2}"
                   ,"#Phi"
                   ,"#Phi"
                   ,"m(K^{#plus}K^{#minus})"
                   ,"m(K^{#plus}K^{#minus})"
                   ,"m(K^{#plus}K^{#minus})"};
  for(int lego = 0; lego < 2; lego++)
  {
    for(int i = 0; i < 6; i++)
    {
      canv[i] = new TCanvas(("projection"+comb[i]).c_str(),"",500,500);
      canv[i]->Draw();
      canv[i]->cd();
      tree->Draw((comb[i]+">>htemp("+opts[i]+")").c_str(),"", (lego ? "LEGO" : "COLZ"));
      TH2F* graph = (TH2F*)canv[i]->GetPrimitive("htemp");
      graph->SetTitle("");
      graph->GetXaxis()->SetTitle(xtit[i].c_str());
      graph->GetYaxis()->SetTitle(ytit[i].c_str());
      graph->SetMinimum(0);
      cout << "Fetched graph" << endl;
      string outname = comb[i];
      std::replace(outname.begin(),outname.end(), ':', '-');
      canv[i]->SaveAs((outname + (lego ? "lego.pdf" : ".pdf")).c_str());
    }
  }
  
  
//  tree->Draw();
}
int main(int argc, char* argv[])
{
  if(argc==1)
  {
    PlotBackground("BsphiKK_sideband_mvacut.root");
    return 0;
  }
  else if(argc>2)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  PlotBackground((string)argv[1]);
  return 0;
}
