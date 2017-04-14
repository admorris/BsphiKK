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

#include <string>
#include <iostream>
#include <memory>

void PlotAngAcc(std::string filename, std::string trailer)
{
  auto file = std::unique_ptr<TFile>(TFile::Open(filename.c_str()));
  TTree* dataacctree = (TTree*)file->Get("dataacctuple");
  TTree* sampledtree = (TTree*)file->Get("sampledtuple");
  gStyle->SetOptStat(0);
  std::string varname[4] = {"mKK","phi","ctheta_1","ctheta_2"};
  std::string axistitle[4] = { "#it{m}(#it{K}^{#plus}#it{K}^{#minus}) [MeV/#it{c}^{2}]"
                        , "#it{#Phi}"
                        , "cos #it{#theta_{1}}"
                        , "cos #it{#theta_{2}}"
                        };
  std::string unit[4] = { " MeV/#it{c}^{2}", "", "", "" };
  double minima[4] = {0.980, -M_PI, -1, -1};
  double maxima[4] = {1.800, +M_PI, +1, +1};
  int nbins[4] = {30, 30, 30, 30};
  std::map<std::string,std::string> titlemap;
  float textsize = 0.055;
  std::cout << "Drawing 1D plots" << std::endl;
  for(int i = 0; i < 4; i++)
  {
    titlemap[varname[i]] = axistitle[i];
    TCanvas canv1d((varname[i]+"1Dplot").c_str(),"",1200,900);
    int nsamples = 10;
    TH1D AccData((varname[i]+"AccData").c_str(),"",nbins[i],minima[i],maxima[i]);
    TH1D AccProj((varname[i]+"AccProj").c_str(),"",nbins[i]*nsamples,minima[i],maxima[i]);
    canv1d.SetFillColor(kWhite);
    TPad pad;
    // left right bottom top
    pad.SetMargin(0.15,0.05,0.15,0.05);
    pad.SetTicks(1,1);
    pad.SetFillColor(kWhite);
    pad.Draw();
    pad.cd();
    dataacctree->Draw((varname[i]+">>"+AccData.GetName()).c_str(),"weight","E1");
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
    std::string option = varname[i] == "mKK" ? "HIST SAME L" : "HIST SAME C";
    sampledtree->Draw((varname[i]+">>"+AccProj.GetName()).c_str(),"weight",option.c_str());
    AccProj.Scale(AccData.Integral()*(double)nsamples/AccProj.Integral());
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
    std::string _blurbtext     = "#splitline{LHCb}{#scale[0.75]{Preliminary}}";
    TLatex _blurb(_blurbx,_blurby,_blurbtext.c_str());
    _blurb.SetTextFont(132);
    _blurb.SetTextColor(1);
    _blurb.SetNDC(kTRUE);
    _blurb.SetTextAlign(11);
    _blurb.SetTextSize(0.07);
    _blurb.Draw();
    std::string plotfilename = "acceptance_"+varname[i]+"_proj";
    if(trailer != "") plotfilename += "_" + trailer;
    canv1d.SaveAs((plotfilename+".pdf").c_str());
  }
  const std::vector<std::string> combinations = {"ctheta_1:ctheta_2","ctheta_1:phi","ctheta_2:phi","phi:mKK","ctheta_1:mKK","ctheta_2:mKK"};
  for(auto comb : combinations)
  {
    TCanvas canv(("projection"+comb).c_str(),"",500,500);
    std::cout << "Drawing " << comb << std::endl;
    canv.cd();
    std::string::size_type separator = comb.find(':');
    TPad pad("pad","",0,0,1,1);
    pad.SetMargin(0.12,0.08,0.12,0.08);
    pad.Draw();
    pad.cd();
    sampledtree->Draw(comb.c_str(),"weight","COLZ");
    TH2F* graph = (TH2F*)pad.GetPrimitive("htemp");
    graph->SetTitle("");
    graph->GetXaxis()->SetTitleSize(textsize);
    graph->GetXaxis()->SetLabelSize(textsize);
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->SetTitleSize(textsize);
    graph->GetYaxis()->SetLabelSize(textsize);
    graph->GetYaxis()->CenterTitle();
    if(comb.find("mKK")!=std::string::npos)
    {
      graph->GetXaxis()->SetNdivisions(505);
    }
    std::cout << titlemap[comb.substr(0,separator)] << std::endl;
    graph->GetYaxis()->SetTitle(titlemap[comb.substr(0,separator)].c_str());
    graph->GetXaxis()->SetTitle(titlemap[comb.substr(separator+1,comb.size())].c_str());
    std::cout << "Fetched graph" << std::endl;
    std::replace(comb.begin(),comb.end(), ':', '-');
    std::string plotfilename = "acceptance_"+comb;
    if(trailer != "") plotfilename += "_" + trailer;
    canv.SaveAs((plotfilename+".pdf").c_str());
  }
}
int main(int argc, char* argv[])
{
  switch(argc)
  {
  case 1:
    PlotAngAcc("sampled_acceptance.root", "");
    break;
  case 2:
    PlotAngAcc((std::string)argv[1],"");
    break;
  case 3:
    PlotAngAcc((std::string)argv[1],(std::string)argv[2]);
    break;
  default:
    std::cerr << "Too many arguments." << std::endl;
    return 1;
    break;
  }
  return 0;
}
